import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///birthdays.db")

SELECT_ALL_BIRTHDAYS_QUERY = "SELECT * FROM birthdays;"
SELECT_BIRTHDAY_QUERY = "SELECT * FROM birthdays WHERE id = ?"
INSERT_BIRTHDAY_QUERY = "INSERT INTO birthdays (name, month, day) VALUES (?, ?, ?);"
DELETE_BIRTHDAY_QUERY = "DELETE FROM birthdays WHERE id = ?;"
UPDATE_BIRTHDAY_QUERY = "UPDATE birthdays SET name = ?, month = ?, day = ? WHERE id = ?"

INVALID_DATE_MESSAGE = (
    "Could not add/update birthday. Please enter valid month and day values."
)
BIRTHDAY_NOT_FOUND_MESSAGE = "Birthday not found."

VALID_MONTHS = {
    1: "January",
    2: "February",
    3: "March",
    4: "April",
    5: "May",
    6: "June",
    7: "July",
    8: "August",
    9: "September",
    10: "October",
    11: "November",
    12: "December",
}
VALID_DAYS = range(1, 32)


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET", "POST"])
def index():
    if request.method == "POST":
        name = request.form.get("name")
        month = int(request.form.get("month"))
        day = int(request.form.get("day"))

        if month not in VALID_MONTHS or day not in VALID_DAYS:
            return INVALID_DATE_MESSAGE, 400

        db.execute(INSERT_BIRTHDAY_QUERY, name, month, day)
        return redirect("/")
    else:
        birthdays = db.execute(SELECT_ALL_BIRTHDAYS_QUERY)
        return render_template(
            "index.html", birthdays=birthdays, months=list(VALID_MONTHS.values())
        )


@app.route("/delete", methods=["POST"])
def delete():
    id = request.form.get("id")
    db.execute(DELETE_BIRTHDAY_QUERY, id)
    return redirect("/")


@app.route("/update/<int:id>", methods=["GET", "POST"])
def update(id):
    if request.method == "POST":
        name = request.form.get("name")
        month = int(request.form.get("month"))
        day = int(request.form.get("day"))

        if month not in VALID_MONTHS or day not in VALID_DAYS:
            return INVALID_DATE_MESSAGE, 400

        db.execute(UPDATE_BIRTHDAY_QUERY, name, month, day, id)
        return redirect("/")
    else:
        birthday = db.execute(SELECT_BIRTHDAY_QUERY, id)
        if birthday:
            return render_template(
                "update.html", birthday=birthday[0], months=VALID_MONTHS
            )
        else:
            return BIRTHDAY_NOT_FOUND_MESSAGE, 404
