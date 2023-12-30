from flask import Flask, render_template, request

app = Flask(__name__)

colors = ["red", "green"]


@app.route("/", methods=["GET", "POST"])
def index():
    if request.method == "POST":
        color = request.form.get("color")
        if color in colors:
            return render_template("color.html", color=color)
        else:
            return "Invalid color", 400
    else:
        return render_template("index.html")
