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

    stock_holdings = db.execute(
        "SELECT symbol, SUM(CASE WHEN type = 'buy' THEN shares ELSE -shares END) AS total_shares FROM transactions WHERE user_id = ? GROUP BY symbol HAVING total_shares > 0;",
        user_id,
    )

    for holding in stock_holdings:
        symbol = holding["symbol"]
        stock_data = lookup(symbol)
        holding["current_price"] = stock_data["price"]
        holding["total_value"] = holding["total_shares"] * holding["current_price"]

    user_cash = db.execute("SELECT cash FROM users WHERE id = ?;", user_id)[0]["cash"]

    return render_template(
        "index.html", stockholdings=stock_holdings, usercash=user_cash
    )


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    user_id = session["user_id"]

    if request.method == "POST":
        symbol = request.form.get("symbol").strip()
        shares = request.form.get("shares")

        if not symbol or not shares:
            return apology("Fields must not be blank", 400)

        if not symbol.isalnum() or len(symbol) > 5:
            return apology("Invalid symbol", 400)

        if not shares.isdigit() or int(shares) <= 0:
            return apology("Shares must be a positive integer", 400)

        shares = int(shares)

        stock_data = lookup(symbol)
        if not stock_data:
            return apology("Symbol does not exist", 400)

        user_cash = db.execute("SELECT cash FROM users WHERE id = ?;", user_id)[0][
            "cash"
        ]
        price_per_share = stock_data["price"]
        total_value = price_per_share * shares

        if total_value > user_cash:
            return apology("Not enough money for that purchase", 422)

        db.execute(
            "UPDATE users SET cash = cash - ? WHERE id = ?;",
            total_value,
            user_id,
        )
        db.execute(
            "INSERT INTO transactions (user_id, symbol, shares, price_per_share, type) VALUES (?, ?, ?, ?, 'buy');",
            user_id,
            symbol,
            shares,
            price_per_share,
        )

        flash("Bought successfully!", "success")
        return redirect("/")
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    user_id = session["user_id"]

    transactions = db.execute(
        "SELECT type, symbol, shares, price_per_share, date FROM transactions WHERE user_id = ? ORDER BY date DESC;",
        user_id,
    )

    return render_template("history.html", transactions=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        username = request.form.get("username").strip()
        password = request.form.get("password").strip()

        # Ensure username was submitted
        if not username:
            return apology("Must provide username", 403)

        # Ensure password was submitted
        if not password:
            return apology("Must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", username)

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("Invalid username and/or password", 403)

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
    if request.method == "POST":
        symbol = request.form.get("symbol").strip()

        if not symbol:
            return apology("Symbol must not be blank", 400)

        if not symbol.isalnum() or len(symbol) > 5:
            return apology("Invalid symbol", 400)

        stock_data = lookup(symbol)
        if not stock_data:
            return apology("Error fetching stock data", 400)

        return render_template("quoted.html", stockdata=stock_data)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    if request.method == "POST":
        username = request.form.get("username").strip()
        password = request.form.get("password").strip()
        password_confirmation = request.form.get("confirmation").strip()

        if not username or not password or not password_confirmation:
            return apology("Fields must not be blank", 400)

        if password != password_confirmation:
            return apology("Passwords don't match", 400)

        existing_user = db.execute("SELECT * FROM users WHERE username = ?", username)
        if existing_user:
            return apology("Username already exists", 400)

        hashed_password = generate_password_hash(password)

        db.execute(
            "INSERT iNTO users (username, hash) VALUES (?, ?);",
            username,
            hashed_password,
        )

        flash("Account created successfully!", "success")
        return redirect("/")
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    user_id = session["user_id"]

    if request.method == "POST":
        symbol = request.form.get("symbol").strip()
        shares = int(request.form.get("shares"))

        if not symbol:
            return apology("Must select a stock", 400)

        if shares <= 0:
            return apology("Shares must be a positive integer", 400)

        owned_shares = db.execute(
            "SELECT SUM(CASE WHEN type = 'buy' THEN shares ELSE -shares END) AS total_shares FROM transactions WHERE user_id = ? AND symbol = ? GROUP BY symbol;",
            user_id,
            symbol,
        )
        if owned_shares[0]["total_shares"] < shares:
            return apology("Not enough shares", 400)

        stock_data = lookup(symbol)
        if not stock_data:
            return apology("Error fetching stock data", 400)

        price_per_share = stock_data["price"]
        total_value = shares * price_per_share

        db.execute(
            "UPDATE users SET cash = cash + ? WHERE id = ?;",
            total_value,
            user_id,
        )
        db.execute(
            "INSERT INTO transactions (user_id, symbol, shares, price_per_share, type) VALUES (?, ?, ?, ?, 'sell');",
            user_id,
            symbol,
            shares,
            stock_data["price"],
        )

        flash("Sold successfully!", "success")
        return redirect("/")
    else:
        owned_stocks = db.execute(
            "SELECT symbol FROM transactions WHERE user_id = ? GROUP BY symbol HAVING SUM(CASE WHEN type = 'buy' THEN shares ELSE -shares END) > 0;",
            user_id,
        )
        return render_template("sell.html", stocks=owned_stocks)


@app.route("/changepassword", methods=["GET", "POST"])
@login_required
def change_password():
    user_id = session["user_id"]

    if request.method == "POST":
        current_password = request.form.get("current-password")
        new_password = request.form.get("new-password")
        new_password_confirmation = request.form.get("confirmation")

        if not current_password or not new_password or not new_password_confirmation:
            return apology("Fields must not be blank", 400)

        if new_password != new_password_confirmation:
            return apology("Passwords don't match", 400)

        user = db.execute("SELECT hash FROM users WHERE id = ?", user_id)[0]

        if not check_password_hash(user["hash"], current_password):
            return apology("Current password is incorrect", 400)

        hashed_new_password = generate_password_hash(new_password)
        db.execute(
            "UPDATE users SET hash = ? WHERE id = ?", hashed_new_password, user_id
        )

        flash("Password changed successfully!", "success")
        return redirect("/")
    else:
        return render_template("changepassword.html")


@app.route("/addcash", methods=["GET", "POST"])
@login_required
def add_cash():
    user_id = session["user_id"]

    if request.method == "POST":
        amount = float(request.form.get("amount"))

        if amount <= 0:
            return apology("Cash must be positive", 400)

        db.execute(
            "UPDATE users SET cash = cash + ? WHERE id = ?;",
            amount,
            user_id,
        )

        flash(f"{usd(amount)} added successfully!", "success")
        return redirect("/")
    else:
        return render_template("addcash.html")
