This system measures the cpu and network utilization of two hosts, sends the data to a monitor
    through a message broker, and stores and displays the information on the monitor.

Existing virtualhosts: /, netappTeam01
Users: (username:password) only have access to netappTeam01 vhost
    cooperra:cooperrapassword
    antclift1:antclift1password
    sarahkh:sarahkhpassword
    zjwy99:zjwy99password

Host Usage: $ python3 pistatsd.py –b message broker [–p virtual host] [–c login:password] –k routing key
Monitor Usage: $ python3 pistatsview.py –b message broker [–p virtual host] [–c login:password] –k routing key
