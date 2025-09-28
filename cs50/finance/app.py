import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


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

    user = db.execute("SELECT cash FROM users WHERE id = ?", user_id)

    cash = user[0]["cash"]

    rows = db.execute("""
        SELECT symbol, SUM(shares) as total_shares
        FROM transactions
        WHERE user_id = ?
        GROUP BY symbol
        HAVING total_shares > 0
    """, user_id)

    portfolio = []
    total_value = 0

    for row in rows:
        symbol = row["symbol"]
        shares = row["total_shares"]
        stock = lookup(symbol)
        price = stock["price"]
        name = stock["name"]
        value = shares * price
        total_value += value

        portfolio.append({
            "symbol": symbol,
            "name": name,
            "shares": shares,
            "price": price,
            "total": value
        })

    grand_total = total_value + cash

    return render_template("index.html", portfolio=portfolio, cash=cash, total=grand_total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    if request.method == "POST":
        symbol = request.form.get("symbol").upper()
        shares = request.form.get("shares")

        if not symbol:
            return apology("Missing symbol")
        if not shares or not shares.isdigit() or int(shares) <= 0:
            return apology("Invalid shares")

        shares = int(shares)

        stock = lookup(symbol)
        if stock is None:
            return apology("Invalid symbol")

        price = stock["price"]
        total_cost = shares * price

        user = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
        cash = user[0]["cash"]

        if cash < total_cost:
            return apology("Can't afford")

        db.execute(
            "INSERT INTO transactions (user_id, symbol, shares, price, type) VALUES (?, ?, ?, ?, ?)",
            session["user_id"], symbol, shares, price, "buy"
        )
        db.execute("UPDATE users SET cash = cash - ? WHERE id = ?", total_cost, session["user_id"])

        return redirect("/")
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    user_id = session["user_id"]

    # Get all transactions for the current user
    transactions = db.execute("""
        SELECT symbol, shares, price, timestamp
        FROM transactions
        WHERE user_id = ?
        ORDER BY timestamp DESC
    """, user_id)

    return render_template("history.html", transactions=transactions)


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
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/register", methods=["GET", "POST"])
def register():
    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        # Check for missing fields
        if not username:
            return apology("must provide username", 400)
        if not password or not confirmation:
            return apology("must provide password and confirmation", 400)
        if password != confirmation:
            return apology("passwords do not match", 400)

        # Hash the password
        hash_pw = generate_password_hash(password)

        # Try to insert new user into the database
        try:
            db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", username, hash_pw)
        except ValueError:
            return apology("username already exists", 400)

        # Log the user in automatically
        user_id = db.execute("SELECT id FROM users WHERE username = ?", username)[0]["id"]
        session["user_id"] = user_id

        return redirect("/")

    else:
        return render_template("register.html")

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
    if request.method == "POST":
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("must provide symbol")

        stock = lookup(symbol)
        if stock is None:
            return apology("invalid symbol")

        return render_template("quoted.html", stock=stock)

    else:
        return render_template("quote.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    if request.method == "GET":
        stocks = db.execute("""
            SELECT symbol, SUM(shares) AS total_shares
            FROM transactions
            WHERE user_id = ?
            GROUP BY symbol
            HAVING total_shares > 0
        """, session["user_id"])
        return render_template("sell.html", stocks=stocks)

    # POST
    symbol = request.form.get("symbol")
    shares_to_sell = request.form.get("shares")

    if not symbol:
        return apology("must provide symbol", 400)
    if not shares_to_sell or not shares_to_sell.isdigit():
        return apology("must provide a positive integer", 400)

    shares_to_sell = int(shares_to_sell)
    if shares_to_sell <= 0:
        return apology("invalid number of shares", 400)

    # Check if the user owns enough shares
    rows = db.execute("""
        SELECT SUM(shares) AS total_shares
        FROM transactions
        WHERE user_id = ? AND symbol = ?
        GROUP BY symbol
    """, session["user_id"], symbol)

    if len(rows) != 1 or rows[0]["total_shares"] < shares_to_sell:
        return apology("not enough shares", 400)

    # Lookup current stock price
    quote = lookup(symbol)
    if quote is None:
        return apology("invalid symbol", 400)

    price = quote["price"]
    total = price * shares_to_sell

    # Insert sale as negative shares
    db.execute("""
        INSERT INTO transactions (user_id, symbol, shares, price, type)
        VALUES (?, ?, ?, ?, ?)
    """, session["user_id"], symbol, -shares_to_sell, price, "SELL")

    # Update user's cash
    db.execute("""
        UPDATE users SET cash = cash + ? WHERE id = ?
    """, total, session["user_id"])

    flash("Sold!")
    return redirect("/")
