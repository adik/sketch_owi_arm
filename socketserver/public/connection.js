var socket;
var holded = false;

function initSocket() 
{
  socket = new WebSocket('ws://'+(location.hostname)+':8080');
  socket.onmessage = function(mess) {
    $('#debug').text(mess.data);
  };

  $("#control1 input[type=button]").bind ({
  	mousedown : function(e) {
    	down($(this));
  	},
  	mouseup : function(e) {
    	if (holded) up($(this));
  	},
  	mouseleave : function(e) {
    	if (holded) up($(this));
  	}
  });

  $("#setpos").bind("click", coord );
}

function down(o) {
    $("#debug2").text("down")
	socket.send('down ' + o.attr('name'))
	holded = true;
}

function up(o) {
	$("#debug2").text("down")
	socket.send('up ' + o.attr('name'))
	holded = false;
}

function coord(){
  socket.send('pos ' + $("#mot0").val() + ',' + $("#mot1").val() + ',' +  $("#mot2").val() + ',' + $("#mot3").val() + ' 0')
}

function set_coord(arr) {
	for(i=0;i<4;i++) {
		$("#mot"+i).val(Math.round(arr[i]));
	} 
}
