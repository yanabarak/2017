from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import gettempdir

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = gettempdir()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route("/")
@login_required
def index():
    user = db.execute("SELECT * FROM users WHERE id = :user_id", user_id=session["user_id"])
    cash = user[0]["cash"]
    total = cash
    
    deals = db.execute("SELECT symbol,price, SUM(shares) AS shares FROM transactions WHERE user_id = :user_id GROUP BY symbol HAVING SUM(shares) > 0", user_id=session["user_id"])
    
    for deal in deals:
        quote = lookup(deal["symbol"])
        deal["price"] = deal["price"]
        deal["name"] = quote["name"]
        deal["amount"] = quote["price"] * deal["shares"]
        total += deal["amount"]
        
    return render_template("index.html", deals=deals, cash=cash, total=total)

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""
    
    if request.method == "POST":

        if not request.form.get("symbol"):
            return apology("Sorry! You must write symbol")
        
        if not request.form.get("shares"):
            return apology("Sorry! You must write shares")
        
        if not request.form.get("shares").isdigit():
            return apology("invalid shares")
            
        quote = lookup(request.form.get("symbol"))
        if not quote:
            return apology("invalid symbol")
        
        symbol = quote["symbol"]
        shares = int(request.form.get("shares"))
        price = quote["price"]
        cost = price * shares
        
        rows = db.execute("SELECT cash FROM users WHERE id=:id", id = session["user_id"])
        cash = rows[0]["cash"]
        if cash < cost:
            return apology("buying", "impossible")
            
        db.execute("INSERT INTO transactions (user_id, symbol, price, shares) VALUES(:user_id, :symbol, :price, :shares)", 
            user_id=session["user_id"], symbol=symbol, price=price, shares=shares)
        
        cash -= cost
        db.execute("UPDATE users SET cash = :cash WHERE id = :id", cash=cash, id=session["user_id"])
        
        flash("Buying successful")
        # redirect user to home page
        return redirect(url_for("index"))

    else:
        return render_template("buy.html")

@app.route("/history")
@login_required
def history():
    """Show history of transactions."""
    
    transactions = db.execute("SELECT * FROM transactions WHERE user_id = :user_id", user_id=session["user_id"])
    return render_template("history.html", transactions=transactions)

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("Sorry! You must write username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("Sorry! You must write password")
            
        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        if not request.form.get("symbol"):
            return apology("Sorry! You must write symbol")

        quote = lookup(request.form.get("symbol"))
        if not quote:
            return apology("invalid symbol")
        return render_template("quoted.html",quote=quote)
    else:
        return render_template("quote.html")

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""
    
    session.clear()
    if request.method == "POST":
        if not request.form.get("username"):
            return apology("Sorry! You must write username")
        elif not request.form.get("password1"):
            return apology("Sorry! You must write password")
        elif request.form.get("password2") != request.form.get("password1"):
            return apology("error confirm password")
            
        hash = pwd_context.encrypt(request.form.get("password1"))
        
        id = db.execute("INSERT INTO users (username, hash) VALUES(:username, :hash)", username=request.form.get("username"), hash = hash)
        if not id:
            return apology("this username", "occupied")
        session["user_id"] = id
        
        flash("Registered!")
        return redirect(url_for("index"))

    else:
        return render_template("register.html")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""
    
    if request.method == "POST":

        if not request.form.get("symbol"):
            return apology("Sorry! You must write symbol")
        
        if not request.form.get("shares"):
            return apology("Sorry! You must write shares")
        
        if not request.form.get("shares").isdigit():
            return apology("invalid shares")
            
        symbol = request.form.get("symbol").upper()
        shares = int(request.form.get("shares"))
        
        rows = db.execute("SELECT SUM(shares) AS shares FROM transactions WHERE user_id = :user_id AND symbol = :symbol GROUP BY symbol", user_id = session["user_id"], symbol=symbol)
        if not rows:
            return apology("You do not have these shares")
        if shares > rows[0]["shares"]:
            return apology("You do not have so many shares")
            
        quote = lookup(symbol)
        db.execute("INSERT INTO transactions (user_id, symbol, price, shares) VALUES(:user_id, :symbol, :price, :shares)", 
            user_id=session["user_id"], symbol=symbol, price=quote["price"], shares=-shares)
        
        cost = quote["price"] * shares
        db.execute("UPDATE users SET cash = cash + :cost WHERE id = :id", cost=cost, id=session["user_id"])
        
        flash("Successful sale")
        return redirect(url_for("index"))
    else:
        return render_template("sell.html")
        
@app.route("/deposit", methods=["GET", "POST"])
@login_required
def deposit():
    """Deposit money into the account."""
    
    if request.method == "POST":
        if not request.form.get("sum"):
            return apology("Sorry! You must write sum")
        if not request.form.get("sum").isdigit():
            return apology("invalid sum")
        
        deposit =int(request.form.get("sum"))
        db.execute("UPDATE users SET cash = cash + :deposit WHERE id = :id", deposit=deposit, id=session["user_id"])
        
        flash("Deposit success!")
        return redirect(url_for("index"))
    else:
        return render_template("deposit.html")
