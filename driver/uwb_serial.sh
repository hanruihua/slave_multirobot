echo  'KERNEL=="ttyUSB*", ATTRS{idVendor}=="10c4", ATTRS{idProduct}=="ea60", MODE:="0777", GROUP:="dialout",  SYMLINK+="uwb_node"' >/etc/udev/rules.d/uwb_node.rules

echo 'KERNEL=="ttyUSB[0-9]*",MODE="0666"' >/etc/udev/rules.d/permission.rules

service udev reload
sleep 2
service udev restart
