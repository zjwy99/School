accounts.py runs the interface to the accounts database
    usage: python3 accounts.py <ip address of message broker>
    Requires a mongodb instance running on localhost of the module with this program
    Uses default credentials to access the message broker

recipe.py runs the interface to the inventory database and recipe API
    usage: python3 recipe.py <ip address of message broker>
    Requires a mongodb instance running on localhost of the module with this program
    Uses default credentials to access the message broker

server.py runs the interface between the iPhone app and the message broker
    usage: export FLASK_APP=server.py
           flask run -- host=0.0.0.0
    Needs the IP address of the message broker in the code

The iPhone app requires an iOS 10.3 or higher
    requires the ip address of the web server for the requests
