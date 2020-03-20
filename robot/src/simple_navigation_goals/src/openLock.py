from flask import Flask

app = Flask(__name__)

@app.route("/")
def home():
    return "Hello, World!"
    
@app.route("/openlock")
def openlock():
    return "lock opened"
    
if __name__ == "__main__":
    app.run(host='0.0.0.0', port=4010)