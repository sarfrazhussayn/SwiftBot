from flask import Flask
app = Flask(__name__)
from flask import request

@app.route('/openlock')

def openlock():
    isAuth = request.args.get('auth')
    print("inp", isAuth)
    if(isAuth=="true"):
        file1 = open("lockstatus.txt","w")
        file1.write("opened")
        file1.close() 
        #open_lock("open") 
        return "true"
    else:
        return "false"

if __name__ == '__main__':
    app.run(host= '0.0.0.0',port=4010)
