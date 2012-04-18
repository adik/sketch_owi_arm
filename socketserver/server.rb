#!/usr/bin/env ruby

require 'em-websocket'
require 'serialport'


#params for serial port
port_str = "/dev/ttyACM0"  #may be different for you
baud_rate = 115200
data_bits = 8
stop_bits = 1
parity = SerialPort::NONE

sp = SerialPort.new(port_str, baud_rate, data_bits, stop_bits, parity)
sp.sync = true
sp.read_timeout = 100

sleep 3;

Thread.new {
    while true do
      #puts sp.getc.to_s(16)
      printf("%c", sp.getc)
    end
}


EventMachine::WebSocket.start(:host => "0.0.0.0", :port => 8080) do |ws|
  ws.onopen { 
    ws.send "Hello Client!"
  }
  ws.onmessage { |msg| 
    #puts "Get message: #{msg}"
    
    (command, direction, motornum) = msg.split
    puts "command=#{command}, direction=#{direction}"
    
    motornum = motornum.to_i-1;

	
    if command == 'pos'
      ws.send "POS"
      motor_pos = direction.split(',').map(&:to_i)
      motor_pos.each_with_index{ |pos, i|
      	puts "#{i}, #{pos.to_s(16)}"
      	buff = [0, 1, i, pos]
      	sp.write buff.pack("CCCn*"); sp.flush();
      }
    elsif command != 'down'
      sp.write "\x00\x03"+motornum.chr; sp.flush();
      ws.send "STOP"
    else
      case direction
      when 'left':
        sp.write ["\x00\x02", motornum.chr, "\x01"].to_s; sp.flush();
        ws.send "FORWARD"     
      when 'right':
        sp.write ["\x00\x02", motornum.chr, "\x02"].to_s; sp.flush();
        ws.send "BACKWARD"     
      end
    end
      

  }
  ws.onclose { 
    puts "WebSocket closed" 
  }
end
