

var SCREEN_WIDTH = 280;
var SCREEN_HEIGHT = 280;

var RADIUS = 130;

var RADIUS_SCALE = 1;
var RADIUS_SCALE_MIN = 1;
var RADIUS_SCALE_MAX = 1.5;

// The number of particles that are used to generate the trail
// 1. Pull aura level from page span with actual number

var canvas;
var context;
var particles;

var auraValue;

var mouseX = 140;
var mouseY = 140;
var mouseIsDown = false;

(function ($) {
  // Jquery onload function.
  $(document).ready(function(){

	canvas = document.getElementById( 'aura' );
	
	
	if (canvas && canvas.getContext) {
		context = canvas.getContext('2d');
		
		// Register event listeners
		//document.addEventListener('mousemove', documentMouseMoveHandler, false);
		//document.addEventListener('mousedown', documentMouseDownHandler, false);
		//document.addEventListener('mouseup', documentMouseUpHandler, false);
		//canvas.addEventListener('touchstart', canvasTouchStartHandler, false);
		//canvas.addEventListener('touchmove', canvasTouchMoveHandler, false);
		//window.addEventListener('resize', windowResizeHandler, false);
		
		createParticles();
		
		windowResizeHandler();
		
		setInterval( loop, 1000 / 60 );
	}
  });
})(jQuery); 

function createParticles() {
	particles = [];
	
	// Get aura level from value put on profile by uprise_aura_points.module
	// .textContent works on Firefox -- .innerText works on all other browsers
	auraValue = document.getElementById( 'aura-level-value' ).innerText || document.getElementById( 'aura-level-value' ).textContent;
	
	var QUANTITY = auraValue;

	//var QUANTITY=20;
	
	for (var i = 0; i < QUANTITY; i++) {
		var particle = {
			position: { x: mouseX, y: mouseY },
			shift: { x: mouseX, y: mouseY },
			size: 1,
			angle: 0,
			speed: 0.01+Math.random()*0.04,
			targetSize: 1,
			fillColor: '#' + (Math.random() * 0x404040 + 0xaaaaaa | 0).toString(16),
			orbit: RADIUS*.5 + (RADIUS * .5 * Math.random())
		};
		
		particles.push( particle );
	}
}

function documentMouseMoveHandler(event) {
	mouseX = event.clientX - (window.innerWidth - SCREEN_WIDTH) * .5;
	mouseY = event.clientY - (window.innerHeight - SCREEN_HEIGHT) * .5;
}

function documentMouseDownHandler(event) {
	mouseIsDown = true;
}

function documentMouseUpHandler(event) {
	mouseIsDown = false;
}

function canvasTouchStartHandler(event) {
	if(event.touches.length == 1) {
		event.preventDefault();

		mouseX = event.touches[0].pageX - (window.innerWidth - SCREEN_WIDTH) * .5;
		mouseY = event.touches[0].pageY - (window.innerHeight - SCREEN_HEIGHT) * .5;
	}
}

function canvasTouchMoveHandler(event) {
	if(event.touches.length == 1) {
		event.preventDefault();

		mouseX = event.touches[0].pageX - (window.innerWidth - SCREEN_WIDTH) * .5;
		mouseY = event.touches[0].pageY - (window.innerHeight - SCREEN_HEIGHT) * .5;
	}
}

function windowResizeHandler() {
	//SCREEN_WIDTH = window.innerWidth;
	//SCREEN_HEIGHT = window.innerHeight;
	
	canvas.width = SCREEN_WIDTH;
	canvas.height = SCREEN_HEIGHT;
	
	//canvas.style.position = 'absolute';
	//canvas.style.left = 190 + 'px';
	//canvas.style.top = 240 + 'px';
}

function loop() {
	
	if( mouseIsDown ) {
		// Scale upward to the max scale
		RADIUS_SCALE += ( RADIUS_SCALE_MAX - RADIUS_SCALE ) * (0.02);
	}
	else {
		// Scale downward to the min scale
		RADIUS_SCALE -= ( RADIUS_SCALE - RADIUS_SCALE_MIN ) * (0.02);
	}
	
	RADIUS_SCALE = Math.min( RADIUS_SCALE, RADIUS_SCALE_MAX );
	
	// Fade out the lines slowly by drawing a rectangle over the entire canvas
	context.fillStyle = 'rgba(0,0,0,0.05)';
	context.fillRect(0, 0, context.canvas.width, context.canvas.height);
	
	for (i = 0, len = particles.length; i < len; i++) {
		var particle = particles[i];
		
		var lp = { x: particle.position.x, y: particle.position.y };
		
		// Offset the angle to keep the spin going
		particle.angle += particle.speed;
		
		// Follow mouse with some lag
		particle.shift.x += ( mouseX - particle.shift.x) ;
		particle.shift.y += ( mouseY - particle.shift.y) ;
		
		// Apply position
		particle.position.x = particle.shift.x + Math.cos(i + particle.angle) * (particle.orbit*RADIUS_SCALE);
		particle.position.y = particle.shift.y + Math.sin(i + particle.angle) * (particle.orbit*RADIUS_SCALE);
		
		// Limit to screen bounds
		particle.position.x = Math.max( Math.min( particle.position.x, SCREEN_WIDTH ), 0 );
		particle.position.y = Math.max( Math.min( particle.position.y, SCREEN_HEIGHT ), 0 );
		
		particle.size += ( particle.targetSize - particle.size ) * 0.04;
		
		// If we're at the target size, set a new one. Think of it like a regular day at work.
		// --------------------Makes the particle larger
		if( Math.round( particle.size ) == Math.round( particle.targetSize ) ) {
			particle.targetSize = 1 + Math.random() * 6;
		}
		
		context.beginPath();
		context.fillStyle = particle.fillColor;
		context.strokeStyle = particle.fillColor;
		context.lineWidth = particle.size;
		context.moveTo(lp.x, lp.y);
		context.lineTo(particle.position.x, particle.position.y);
		context.stroke();
		context.arc(particle.position.x, particle.position.y, particle.size/2, 0, Math.PI*2, true);
		context.fill();
	}
}



