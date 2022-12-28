import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import datetime

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    user_id = session["user_id"]

    user_transactions = db.execute("SELECT * FROM active_portfolio WHERE user_id = ?", user_id)
    cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)
    cash = cash[0]["cash"]

    sum = cash
    for row in user_transactions:
        look = lookup(row['symbol'])
        row['name'] = look['name']
        row['price'] = look['price']
        row['total'] = row['price'] * row['shares']

        # increment sum
        sum += row['total']

        # convert price and total to usd format
        row['total'] = row['total']

    return render_template("index.html", transactions=user_transactions, cash=cash, lookup=lookup, usd = usd, sum=sum)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    # if the form is submitted via post
    if request.method == "POST":
        # retrieve the symbol, shares, and stock price as vars. considers ease of use
        symbol = request.form.get("symbol").upper()
        shares = request.form.get("shares")
        stock_info = lookup(symbol)

        # handles fractional shares if they manage to slip through
        if not shares.isdigit():
            return apology("You cannot purchase partial shares.")

        shares = int(shares)
        # return apology if symbol is not provided or does not exist
        if stock_info == None:
            return apology("symbol is not provided or does not exist", 400)


        # return apology if shares are not provided. (the html input already ensures that the min value for shares is 1)
        if not shares:
            return apology("must provide valid number of shares", 400)

        # apology(stock_info)

        price = stock_info["price"]     # retrieves the price value from lookup dict
        user_id = session["user_id"]
        purchase = shares * price
        balance = db.execute("SELECT cash FROM users WHERE id == ?", user_id)[0]["cash"]

        if purchase > balance:
            return apology("Insufficient Funds", 400)

        # query portfolio table for row with this user_id and stock symbol:
        row = db.execute("SELECT * FROM active_portfolio WHERE user_id = ? AND symbol = ?",
                         user_id, symbol)

        # if row doesn't exist yet, create it but don't update shares
        if len(row) != 1:
            db.execute("INSERT INTO active_portfolio (user_id, symbol, shares) VALUES (?, ?, ?)", user_id, symbol, shares)
        else:
            # get previous number of shares owned
            oldshares = row[0]["shares"]

            # add purchased shares to previous share number
            newshares = oldshares + shares

            # update shares in portfolio table
            db.execute("UPDATE active_portfolio SET shares = ? WHERE user_id = ? AND symbol = ?",newshares, user_id, symbol)

        new_balance = balance - purchase
        db.execute("UPDATE users SET cash = ? WHERE id = ?", new_balance, user_id)

        date = datetime.now()
        db.execute("INSERT INTO transactions (user_id, symbol, shares, price, date, method) VALUES(?, ?, ?, ?, ?, ?)", user_id, symbol, shares, price, date, "BUY")
        #db.execute("INSERT INTO active_portfolio (user_id, symbol, shares) VALUES(?, ?, ?)", user_id, symbol, shares)
        flash("Bought!")
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("buy.html")

    #return apology("TODO")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    user_id = session["user_id"]
    user_transactions = db.execute("SELECT * FROM transactions WHERE user_id = ?", user_id)
    return render_template("history.html", transactions = user_transactions, usd = usd)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":

        stock_info = lookup(request.form.get("symbol").upper())

        if stock_info == None:
            return apology("Invalid Symbol", 400)

        return render_template("quoted.html", stock_info=stock_info, usd = usd)

    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        elif request.form.get("password") != request.form.get("confirmation"):
            return apology("passwords must match!", 400)

        password = request.form.get("password")

        passes_tests = False
        required_chars = ["!", "@", "#", "$"]
        for char in password:
            if char in required_chars:
                passes_tests = True
        if passes_tests == False:
            return apology("Password must contain !, @, #, or $", 400)


        # Stores user and password (hashed)
        user = request.form.get("username")
        hash = generate_password_hash(request.form.get("password"))

        # Checks that the username chosen is not already taken
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        if len(rows) != 0:
            return apology("Username is taken. Please try again!", 400)
        new_user = db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", user, hash)

        session["user_id"] = new_user

        # Redirect user to home page
        flash("Registered!")
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")



@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    if request.method == "POST":

        # housekeeping
        user_id = session["user_id"]
        date = datetime.now()

        # get the symbol and shares from the html form
        symbol = request.form.get("symbol")
        shares = int(request.form.get("shares"))
        quote = lookup(symbol)

        # returns a list of stock info for stocks in the user's portfolio that contain the requested symbol
        active_stocks = db.execute("SELECT * FROM active_portfolio WHERE user_id = ? AND symbol = ?", user_id, symbol)

        # return apology if the symbol is invalid/not owned
        if len(active_stocks) != 1:
            return apology(str(symbol), 400)

        # if no shares are provided in the form return apology
        if not shares:
            return apology("must provide number of shares", 403)

        active_shares = active_stocks[0]["shares"]
        # if the shares requested to sell exceed shares owned, return an apology
        if active_shares < shares:
            return apology("Shares sold cannot exceed shares owned!", 400)

        price = quote["price"]
        sell_price = price*shares

        cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)
        cash = cash[0]['cash']
        cash = cash + sell_price

        # sold shares are shown as negative values in the transactions table
        shares *= -1

        db.execute("UPDATE users SET cash = ? WHERE id = ?", cash, user_id)
        db.execute("INSERT INTO transactions (user_id, symbol, shares, price, date, method) VALUES(?, ?, ?, ?, ?, ?)", user_id, symbol, shares, quote["price"], date, "SELL")


        if active_shares == shares:
            db.execute("DELETE FROM active_portfolio WHERE symbol = ? AND user_id = ?",symbol, user_id)
        else:
            new_shares = active_shares+shares
            db.execute("UPDATE active_portfolio SET shares = ? WHERE symbol = ? AND user_id = ?", new_shares, symbol, user_id)

        flash("Sold!")
        return redirect("/")

    else:
        symbols = db.execute("SELECT DISTINCT symbol FROM active_portfolio WHERE user_id = ?", session["user_id"])

        return render_template("sell.html", symbols = symbols)
