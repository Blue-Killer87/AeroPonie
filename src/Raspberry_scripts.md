**Pro připojení na SSH**
hostname: raspberrypi.local
username: pi
heslo: raspberry

**Pro instalaci NodeRed**
bash <(curl -sL https://raw.githubusercontent.com/node-red/linux-installers/master/deb/update-nodejs-and-nodered)

**Pro spuštění NodeRed lokálně s omezovačem na kapacitu**
node-red-pi --max-old-space-size=256

**Node-Red autostart při bootu**
sudo systemctl enable nodered.service

**Defaultní adresa pro NodeRed**
http://127.0.0.1:1880/

**Defaultní adresa pro WLAN**
192.168.50.246
