

var ArmRadar = function (config) 
{
	
	this.canvasStage = new Kinetic.Stage(config);
	this.mainLayer = new Kinetic.Layer();
	this.messageLayer = new Kinetic.Layer();
		
	var circle = new Kinetic.Circle({
		x: this.canvasStage.getWidth() / 2,
		y: this.canvasStage.getHeight(),
		radius: 250,
		fill: "red",
		stroke: "black",
		strokeWidth: 4
	});

	// add the shape to the layer
	this.mainLayer.add(circle);
	
	// add the layer to the stage
	this.canvasStage.add(this.mainLayer);
	this.canvasStage.add(this.messageLayer);
}


ArmRadar.prototype = {
}



var canvasStage = {}
var mainLayer = {}
var messageLayer = {} 


function writeMessage(messageLayer, message) 
{
	var context = messageLayer.getContext();
	messageLayer.clear();
	context.font = "18pt Calibri";
	context.fillStyle = "black";
	context.fillText(message, 10, 25);
}

function initCanvas() 
{
	canvasStage = new Kinetic.Stage({
		container: "canvasContainer",
		width: 510,
		height: 300
	});
	
	mainLayer = new Kinetic.Layer();
	messageLayer = new Kinetic.Layer();
		
	var circle = new Kinetic.Circle({
		x: canvasStage.getWidth() / 2,
		y: canvasStage.getHeight(),
		radius: 250,
		fill: "red",
		stroke: "black",
		strokeWidth: 4
	});

	circle.on("mouseout", function() {
		writeMessage(messageLayer, "Mouseout");
	});
	
	circle.on("mousemove", function() {
		var mousePos = canvasStage.getMousePosition();
		var x = mousePos.x;
		var y = mousePos.y;
		writeMessage(messageLayer, "x: " + x + ", y: " + y);
	});

	// add the shape to the layer
	mainLayer.add(circle);
	
	// add the layer to the stage
	canvasStage.add(mainLayer);
	canvasStage.add(messageLayer);
}

