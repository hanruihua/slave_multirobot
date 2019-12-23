echo  'KERNEL=="ttyUSB*", ATTRS{idVendor}=="10c4", ATTRS{idProduct}=="ea60", MODE:="0777", GROUP:="dialout",  SYMLINK+="uwb_node"' >/etc/udev/rules.d/uwb_node.rules
echo "map the uwb serial port name as /dev/uwb_node"

echo 'KERNEL=="ttyUSB[0-9]*",MODE="0666"' >/etc/udev/rules.d/permission.rules
echo "grant the authority to serial ttyUSB*"

service udev reload
echo "reload the dev"

sleep 2
service udev restart
echo "successfully, restart"
