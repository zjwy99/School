var sketchProc=function(processingInstance){ with (processingInstance){
size(600, 600); 
frameRate(60);
var tileMap = [
    "wwwwwwwwwwwwwwwwwwwwwwwwwwwwww",
    "w                            w",
    "w                            w",
    "wwwwwwwwwwwwwwwwwwwwwwwwwwwwww",
    "w                            w",
    "w                            w",
    "w                            w",
    "w                            w",
    "w                            w",
    "w                            w",
    "w                            w",
    "w                            w",
    "w                            w",
    "w                            w",
    "w                            w",
    "w                            w",
    "w                            w",
    "w                            w",
    "wwwwwwwwwwww                 w",
    "w                            w",
    "w                            w",
    "w                            w",
    "w  w          wwwwwwwwwwwwwwww",
    "w  w                         w",
    "w  w                         w",
    "w  wwwwwwwww                 w",
    "w                            w",
    "w                            w",
    "w                            w",
    "wwwwwwwwwwwwwwwwwwwwwwwwwwwwww"
];

var tilemap2 = [
    "wwwwwwwwwwwwwwwwwwwwwwwwwwwwww",
    "w                            w",
    "w                            w",
    "wwwwwwwwwwwwwwwwwwwwwwwwwwwwww",
    "w                            w",
    "w                            w",
    "w                            w",
    "w                            w",
    "w                      wwwwwww",
    "w                  www       w",
    "w                            w",
    "w             www            w",
    "w                            w",
    "w                            w",
    "w                            w",
    "w   wwwwwwwww        wwwwwwwww",
    "w            ww              w",
    "w                            w",
    "w                            w",
    "w                wwwwwwww    w",
    "w                            w",
    "w                            w",
    "w                            w",
    "wwwwwwwwww    ww             w",
    "w                            w",
    "w                            w",
    "w                            w",
    "w           wwwwwwww         w",
    "w           wwwwwwww         w",
    "wwwwwwwwwwwwwwwwwwwwwwwwwwwwww"
];

var tilemap3 = [
    "wwwwwwwwwwwwwwwwwwwwwwwwwwwwww",
    "w                            w",
    "w                            w",
    "wwwwwwwwwwwwwwwwwwwwwwwwwwwwww",
    "w                            w",
    "w                            w",
    "w                            w",
    "w                            w",
    "w                            w",
    "w                            w",
    "w                            w",
    "w         wwww               w",
    "w                            w",
    "w                            w",
    "w                            w",
    "w   wwww                     w",
    "w                            w",
    "w                            w",
    "w                            w",
    "w          wwww              w",
    "w                            w",
    "w                            w",
    "w                            w",
    "w    wwww                    w",
    "w                            w",
    "w         www                w",
    "w                            w",
    "w                            w",
    "w                            w",
    "wwwwwwwwwwwwwwwwwwwwwwwwwwwwww"
];

var tilemap4 = [
    "wwwwwwwwwwwwwwwwwwwwwwwwwwwwww",
    "w                            w",
    "w                            w",
    "wwwwwwwwwwwwwwwwwwwwwwwwwwwwww",
    "w                            w",
    "w                            w",
    "w                            w",
    "w                            w",
    "w                            w",
    "w                            w",
    "w                            w",
    "ww                          ww",
    "w                            w",
    "w           wwwwww           w",
    "w                            w",
    "w                            w",
    "w    wwwww               wwwww",
    "w                            w",
    "w                            w",
    "w          wwwwww            w",
    "w                            w",
    "w    www           wwwww     w",
    "w                            w",
    "w                            w",
    "w                            w",
    "w          wwwwwww           w",
    "w                            w",
    "w                            w",
    "w                            w",
    "wwwwwwwwwwwwwwwwwwwwwwwwwwwwww"
];

var tilemap5 = [
    "wwwwwwwwwwwwwwwwwwwwwwwwwwwwww",
    "w                            w",
    "w                            w",
    "wwwwwwwwwwwwwwwwwwwwwwwwwwwwww",
    "w                            w",
    "w                            w",
    "w                            w",
    "w                            w",
    "w                            w",
    "w                            w",
    "w                            w",
    "w                            w",
    "w                            w",
    "w                            w",
    "w                            w",
    "w                            w",
    "w                            w",
    "w                            w",
    "w                            w",
    "w                            w",
    "w                            w",
    "w                            w",
    "w                            w",
    "w                            w",
    "w                            w",
    "wwwwww wwwww wwwww wwww wwwwww",
    "w                            w",
    "w                            w",
    "w                            w",
    "wwwwwwwwwwwwwwwwwwwwwwwwwwwwww"
];

var tilemap6 = [
    "wwwwwwwwwwwwwwwwwwwwwwwwwwwwww",
    "w                            w",
    "w                            w",
    "wwwwwwwwwwwwwwwwwwwwwwwwwwwwww",
    "w                            w",
    "w                            w",
    "w                            w",
    "wwwwwwwwwww            wwwwwww",
    "w                            w",
    "w                            w",
    "w                            w",
    "w                            w",
    "w                            w",
    "w                            w",
    "w                            w",
    "w                            w",
    "w                            w",
    "w                            w",
    "wwwwwwwwwwwwwwwwwwwwwww      w",
    "w                            w",
    "w                            w",
    "w                            w",
    "w                            w",
    "w                            w",
    "w                            w",
    "w                            w",
    "w                            w",
    "w                            w",
    "w                            w",
    "wwwwwwwwwwwwwwwwwwwwwwwwwwwwww"
];

var tilemap7 = [
    "wwwwwwwwwwwwwwwwwwwwwwwwwwwwww",
    "w                            w",
    "w                            w",
    "wwwwwwwwwwwwwwwwwwwwwwwwwwwwww",
    "w                            w",
    "w                            w",
    "w                            w",
    "w                            w",
    "w                            w",
    "w                            w",
    "w                            w",
    "w                            w",
    "w                            w",
    "w                            w",
    "w                            w",
    "w                            w",
    "w                            w",
    "w                            w",
    "w                            w",
    "w                            w",
    "w                            w",
    "w                            w",
    "w                            w",
    "w                            w",
    "w                            w",
    "w   w     w   w w      w    ww",
    "w                            w",
    "w                            w",
    "w                            w",
    "wwwwwwwwwwwwwwwwwwwwwwwwwwwwww"
];

var gameState = 1;    
var level = 1;
var currentMap = 0;
var kyp = false;

var wallObj = function(x, y) {
    this.x = x;
    this.y = y;
};
var walls = [];
var allmaps = [];


for (var i=0; i<tileMap.length; i++) {
    for (var j=0; j<tileMap[i].length; j++) {
        if (tileMap[i][j] === 'w') {
            walls.push(new wallObj(j*20, i*20));
        }
    }
}
allmaps.push(walls);
walls = [];

for (var i=0; i<tilemap2.length; i++) {
    for (var j=0; j<tilemap2[i].length; j++) {
        if (tilemap2[i][j] === 'w') {
            walls.push(new wallObj(j*20, i*20));
        }
    }
}

allmaps.push(walls);
walls = [];

for (var i=0; i<tilemap3.length; i++) {
    for (var j=0; j<tilemap3[i].length; j++) {
        if (tilemap3[i][j] === 'w') {
            walls.push(new wallObj(j*20, i*20));
        }
    }
}

allmaps.push(walls);
walls = [];

for (var i=0; i<tilemap4.length; i++) {
    for (var j=0; j<tilemap4[i].length; j++) {
        if (tilemap4[i][j] === 'w') {
            walls.push(new wallObj(j*20, i*20));
        }
    }
}

allmaps.push(walls);
walls = [];

for (var i=0; i<tilemap5.length; i++) {
    for (var j=0; j<tilemap5[i].length; j++) {
        if (tilemap5[i][j] === 'w') {
            walls.push(new wallObj(j*20, i*20));
        }
    }
}

allmaps.push(walls);
walls = [];

for (var i=0; i<tilemap6.length; i++) {
    for (var j=0; j<tilemap6[i].length; j++) {
        if (tilemap6[i][j] === 'w') {
            walls.push(new wallObj(j*20, i*20));
        }
    }
}

allmaps.push(walls);
walls = [];

for (var i=0; i<tilemap7.length; i++) {
    for (var j=0; j<tilemap7[i].length; j++) {
        if (tilemap7[i][j] === 'w') {
            walls.push(new wallObj(j*20, i*20));
        }
    }
}

allmaps.push(walls);
walls = [];

var displayTilemap = function() {
    fill(82, 80, 79);
    noStroke();
    rectMode(CORNER);
    for (var i =0; i<allmaps[currentMap].length; i++) {
        rect(allmaps[currentMap][i].x, allmaps[currentMap][i].y, 20, 20);
    }
};

var pChange = 100;
var pChangeDir = 0;

var playButton = function() {
    fill(100,pChange,pChange);
    strokeWeight(2);
    rect(230,320,150,50);
    fill(0,0,0);
    textSize(35);
    text('Play', 270,355);
	if (mouseX > 230 && mouseX < 380 && mouseY > 320 && mouseY < 370) {
	    if (pChangeDir === 0) {
		    pChange += 2;
	    }
	    if (pChangeDir === 1) {
	        pChange -= 2;    
	    }
		if (pChange > 255 && pChangeDir === 0) {
			pChangeDir = 1;
		}
		if (pChange < 104 && pChangeDir === 1) {
		    pChangeDir = 0;
		}
	}
	else {
	    pChange = 100;    
	}
	
};

var iChange = 100;
var iChangeDir = 0;

var instrButton = function() {
    fill(100,iChange,iChange);
    strokeWeight(2);
    rect(230,420,150,50);
    fill(0,0,0);
    textSize(25);
    text('Instructions',240,455);
    if (mouseX > 230 && mouseX < 380 && mouseY > 420 && mouseY < 470) {
	    if (iChangeDir === 0) {
		    iChange += 2;
	    }
	    if (iChangeDir === 1) {
	        iChange -= 2;    
	    }
		if (iChange > 255 && iChangeDir === 0) {
			iChangeDir = 1;
		}
		if (iChange < 104 && iChangeDir === 1) {
		    iChangeDir = 0;
		}
	}
	else {
	    iChange = 100;    
	}
};


var nChange = 100;
var nChangeDir = 0;

var nextButton = function() {
    fill(100,nChange,nChange);
    strokeWeight(2);
    rect(230,320,150,50);
    fill(0,0,0);
    textSize(25);
    stroke(0, 0, 0);
    if (currentMap === 3) {
        text('Next Level',245,355);
    }
    else if (currentMap === 6) {
        text("Menu", 245, 355);    
    }
    if (mouseX > 230 && mouseX < 380 && mouseY > 320 && mouseY < 370) {
	    if (nChangeDir === 0) {
		    nChange += 2;
	    }
	    if (nChangeDir === 1) {
	        nChange -= 2;    
	    }
		if (nChange > 255 && nChangeDir === 0) {
			nChangeDir = 1;
		}
		if (nChange < 104 && nChangeDir === 1) {
		    nChangeDir = 0;
		}
	}
	else {
	    nChange = 100;    
	}
};

var instructions = function() {
    textFont("impact",20);
    fill(0,0,0);
    text('The basic goal of the game is to move around the level to reach the\ngoal. However, there are enemies scattered around the level trying\nto kill you. Fortunately, you have a gun and can fight back.',0,100);
    text("Movement: You can use the left and right arrow keys to move.\n                   Jumping is done with the up arrow.", 0, 200);
    text("Combat: The space bar is used to fire your gun. You can only carry\na certain ammount of ammo at a time. If you run out of ammo you\ncan press the space bar when indicated to reload.", 0, 300);
};

var rChange = 100;
var rChangeDir = 0;

var returnButton = function() {
    stroke(0,0,0);
    fill(100,rChange,rChange);
    strokeWeight(2);
    rect(230,10,150,50);
    textSize(35);
    fill(0,0,0);
    text('Menu',260,45);
     if (mouseX > 230 && mouseX < 380 && mouseY > 10 && mouseY < 60) {
	    if (rChangeDir === 0) {
		    rChange += 2;
	    }
	    if (rChangeDir === 1) {
	        rChange -= 2;    
	    }
		if (rChange > 255 && rChangeDir === 0) {
			rChangeDir = 1;
		}
		if (rChange < 104 && rChangeDir === 1) {
		    rChangeDir = 0;
		}
	}
	else {
	    rChange = 100;    
	}
};

var explosionObj = function() {
    this.w = 5; 
    this.grow_factor = 0.5;
    this.thickness = 38;
    this.rad = 100;
    this.counter = 0;
    this.duration = 5; 
    this.transparency = 40;
    this.xVal = random(0, this.rad) + this.w;
    this.yVal = random(0, this.rad) + this.w;
    this.count = 0;
    this.done = true;
};

explosionObj.prototype.grow = function(n) {
    return n + n * this.grow_factor;
};

explosionObj.prototype.explodeFrame = function(x, y) {
     for(var i = 0; i <= 275; i ++) {
        var theta = Math.random()*2*Math.PI - Math.PI;
        var x2 = this.xVal + this.w*Math.cos(theta);
        var y2 = this.yVal + this.w*Math.sin(theta);
        stroke(random(200, 255), random(80, 200), 0, this.transparency);
        strokeWeight(this.thickness);
        line(this.xVal, this.yVal, random(this.xVal, x2), random(this.yVal, y2));
    }
};

explosionObj.prototype.explosion = function(x, y) {
    if (this.counter < this.duration ) {
        this.explodeFrame(x, y);
        this.counter = this.counter + 0.1;
        this.w = this.grow(this.w);
    } 
};

var title = function() {
    textFont("impact",60);
    strokeWeight(5);
    fill(255,255,255);
    text('Super Soldier\n   Adventure',120,60);
    fill(0, 0, 0);
    textFont("impact", 30);
    text('Created by Zachary Yee', 130, 200);
};

var missileObj = function(x, y, changeX, changeY) {
    this.x = x;
    this.y = y;
    this.r = 45;
    this.changeX = changeX;
    this.changeY = changeY;
};

missileObj.prototype.draw = function() {
	fill(255, 0, 0);
	noStroke();
    arc(this.x + 6, this.y + 8, 20, 20, 225, 405);
    noStroke();
    fill(255,255,0);
    ellipse(this.x - 38, this.y + 35, 45, 45);
    fill(255, 0, 0);
    ellipse(this.x - 35, this.y + 33, 25, 25);
    noStroke();   
    pushMatrix();
    translate(this.x, this.y);
    rotate(this.r);
    fill(255,140,0);
    triangle(-15, 40, 10, 20, 35, 40);
    noStroke();
    fill(192,192,192);
    rect(0, 0, 20, 50);
    popMatrix();
    noStroke();
    
};

missileObj.prototype.update = function() {
    this.x = this.x + this.changeX;
    this.y = this.y - this.changeY;
};

var enemyObj = function(x, y, screenState) {
    this.x = x;
    this.y = y;
    this.health = 1;
    this.screenState = screenState;
    this.show = true;
    this.enemyDir = 0;
};

enemyObj.prototype.draw = function() {
    if (this.show === true) {
        stroke(0, 0,0);
        strokeWeight(2);
        if (this.health === 1) {
            fill(255,165,0);
        }
        if (this.health === 2) {
            fill(109, 214, 200);
        }
        if (this.health === 3) {
            fill(176, 34, 9);
        }
        if (this.health === 4) {
            fill(24, 184, 29);
        }
        rect(this.x, this.y, 20, 40);
        rect(this.x + 4, this.y - 10, 12, 10);
        strokeWeight(5);
        stroke(255, 0, 0);
        point(this.x + 10, this.y - 5);
        strokeWeight(2);
        stroke(0, 0, 0);
        line(this.x, this.y + 20, this.x + 20, this.y + 20);
        line(this.x + 10, this.y + 10, this.x + 10, this.y + 30);
        line(this.x, this.y + 15, this.x + 20, this.y + 25);
    }
};

enemyObj.prototype.update = function() {
    if (this.screenState === 1) {
        this.x += 0.5;
        if (this.x > 610) {
            this.x = -25;   
        }
    }
};

var missile = new missileObj(0, 400, 0.7, 0.5);
var missile2 = new missileObj(0, 200, 0.7, 0.5);
var missile3 = new missileObj(0, 300, 0.7, 0.5);
var enemy = new enemyObj(0, 560, 1);
var enemy2 = new enemyObj(40, 560, 1);
var enemy3 = new enemyObj(80, 560, 1);

var titleBackgroundAnimation = function() {
    background(12, 69, 102);
    missile.draw();
    missile.update();
    missile2.draw();
    missile2.update();
    missile3.draw();
    missile3.update();
    if (missile.x > 690) {
        missile.x = 0;
        missile.y = 400;
    }
    if (missile2.x > 440) {
        missile2.x = 0;
        missile2.y = 200;
    }
    if (missile3.x > 590) {
        missile3.x = 0;
        missile3.y = 300;
    }
	enemy.draw();
    enemy2.draw();
    enemy3.draw();
    enemy.update();
    enemy2.update();
    enemy3.update();
    
};

var bulletObj = function (x, y) {
    this.x = 0;
    this.y = 0;
    this.speed = 4;
    this.right = true;
    this.fired = false;
    this.used = false;
};

bulletObj.prototype.draw = function() {
    if (this.fired === true) {
        fill(21, 252, 5);
        ellipse(this.x, this.y, 5, 5);
    }
};

bulletObj.prototype.update = function() {
    if (this.fired === true) {
        if (this.right === true) {
            this.x += this.speed;    
        }
        if (this.right === false) {
            this.x -= this.speed;    
        }
        if (this.x > 700 || this.x < 0) {
            this.fired = false;
            this.used = true;
        }
        for (var i = 0; i < allmaps[currentMap].length; i++) {
            if (this.x + 2.5 > allmaps[currentMap][i].x && this.x < allmaps[currentMap][i].x + 20 && this.y + 2.5 > allmaps[currentMap][i].y && this.y < allmaps[currentMap][i].y + 20) {
                this.fired = false;
                this.used = true;
            }
        }
    }
};

var playerObj = function(x, y) {
    this.x = x;
    this.y = y;
    this.jump = false;
    this.gravity = 0.2;
    this.changeY = 1;
    this.changeX = 0;
    this.onground = false;
    this.bullets = [];
    this.ammo = 0;
    this.right = true;
    this.lastGround = 0;
    this.health = 20;
};

var player = new playerObj(110, 100);
for (var i = 0; i < 10; i++) {
    player.bullets[i] = new bulletObj(20, 20);    
}

var enemyBulletObj = function (x, y) {
    this.x = 0;
    this.y = 0;
    this.speed = 4;
    this.yFire = false;
    this.right = true;
    this.fired = false;
    this.used = false;
    this.normal = true;
};

enemyBulletObj.prototype.draw = function() {
    if (this.fired === true) {
        fill(255, 0, 0);
        stroke(0, 0, 0);
        ellipse(this.x, this.y, 5, 5);
    }
};

enemyBulletObj.prototype.update = function() {
    if (this.fired === true) {
        if (this.normal === true) {
            if (this.right === true) {
                this.x += this.speed;    
            }
            if (this.right === false) {
                this.x -= this.speed;    
            }
        }
        if (this.x > 700 || this.x < 0) {
            this.fired = false;
            this.used = true;
        }
        for (var i = 0; i < allmaps[currentMap].length; i++) {
                if (this.x + 2.5 > allmaps[currentMap][i].x && this.x < allmaps[currentMap][i].x + 20 && this.y + 2.5 > allmaps[currentMap][i].y && this.y < allmaps[currentMap][i].y + 20) {
                    this.fired = false;
                    this.used = true;
                }
        }
        if (player.x + 20 > this.x && player.x < this.x + 2.5 && player.y + 40 > this.y && player.y - 10 < this.y + 2.5) {
            player.health -= 1;
            this.fired = false;
            this.used = true;
        }
    }
};




playerObj.prototype.draw = function() {
    stroke(0,0,0);
    strokeWeight(2);
    fill(128,128,128);
    rect(this.x, this.y, 20, 40);
    rect(this.x + 4, this.y - 10, 12, 10);
    stroke(255, 0, 0);
    line(this.x + 7, this.y - 5, this.x + 12, this.y - 5);
    stroke(0,0,0);
    fill(0,255,255);
    if (this.right === true) {
        rect(this.x + 20, this.y + 10, 10, 2);    
    }
    else {
        rect(this.x - 10, this.y + 10, 10, 2);
    }
    kyp = false;
	
};

playerObj.prototype.update = function() {
    if (this.onground === true) {
        this.changeY = 0;    
    }
    if (this.onground === false) {
        this.changeY += this.gravity;
    }
    this.y += this.changeY;
    this.x += this.changeX;
    

};

playerObj.prototype.collision = function() {
    var hit = false;
    
    for (var i = 0; i < allmaps[currentMap].length; i++) {
        if (allmaps[currentMap][i].y < this.y  - 10 && allmaps[currentMap][i].y + 20 > this.y - 12 && allmaps[currentMap][i].x < this.x + 16 && allmaps[currentMap][i].x + 20 > this.x + 4) {
            this.changeY = this.gravity;
            //this.changeX = 0;
            hit = true;
            this.jump = false;
            //return true;
        }
        if (allmaps[currentMap][i].y < this.y + 50 && allmaps[currentMap][i].y + 20 > this.y + 40 && allmaps[currentMap][i].x < this.x + 15 && allmaps[currentMap][i].x + 20 > this.x + 5) {
            this.onground = true;
            this.changeX = 0;
            this.y = allmaps[currentMap][i].y - 40;
            this.lastGround = allmaps[currentMap][i].y - 40;
            hit = true;
            this.changeY = 0;
            this.jump = false;
            return true;
        }
        if (allmaps[currentMap][i].x < this.x + 21 && allmaps[currentMap][i].x + 20 > this.x - 1 && allmaps[currentMap][i].y <= this.y + 35 &&  allmaps[currentMap][i].y + 20 > this.y) {
            var d = this.x - allmaps[currentMap][i].x;
            if (d < 0) {
                this.x = allmaps[currentMap][i].x - 20;
            }
            else if (d > 0) {
                this.x = allmaps[currentMap][i].x + 20;
            }
        }
    }
    this.jump = false;
    this.onground = false;
    return false;
};

var level1Boss = function(x, y) {
    this.x = x;
    this.y = y;
    this.armr = -35;
    this.armDir = 0;
    this.bodyDir = 0;
    this.armx = this.x + 35;
    this.health = 20;
    this.bossBullets = [];
    for (var i = 0; i < 3; i++) {
        this.bossBullets.push(new enemyBulletObj(20, 200));    
    }
    for (var i = 0; i < 3; i++) {
        this.bossBullets[i].normal = false;    
    }
    this.count = 0;
    this.show = true;
};

level1Boss.prototype.draw =  function() {
    if (this.show === true) {
        stroke(0, 0, 0);
        fill(255,215,0);
        
        pushMatrix();
        translate(this.x, this.y);
        rotate(-35);
        rect(-33, -80, 50, 80);
        popMatrix();
        
        ellipseMode(CENTER);
        fill(255,215,0);
        stroke(0, 0, 0);
        ellipse(this.x, this.y, 80, 80);
        noStroke();
        fill(255, 0, 0);
        ellipse(this.x - 5, this.y -5, 25, 25);
        stroke(0, 0, 0);
        fill(255,215,0);
        ellipse(this.x + 60, this.y + 90, 80, 80);
        rect(this.x + 20, this.y + 90, 80, 50);
        stroke(255,215,0);
        line(this.x + 22, this.y + 90, this.x + 97, this.y + 90);
    
        stroke(0, 0, 0);
        rect(this.armx, this.y + 90, 60, 30);
        rect(this.armx - 20, this.y + 95, 20, 20);
        fill(0, 0, 0);
        ellipse(this.armx - 20, this.y + 105, 10, 20);
        
        
        fill(255, 0, 0);
        ellipse(this.x + 33, this.y + 165, 25, 40);
        ellipse(this.x + 88, this.y + 165, 25, 40);
        
        fill(255,215,0);
        rect(this.x + 20, this.y + 140, 25, 20);
        rect(this.x + 75, this.y + 140, 25, 20);
        for (var i = 0; i < 3; i++) {
            this.bossBullets[i].draw();   
        }
    }
};

level1Boss.prototype.update = function() {
    if (this.show === true) {
        if (this.armDir === 0) {
            this.armr += 0.25;
            this.armx -= 0.7;
        }
        if (this.armDir === 1) {
            this.armr -= 0.25;
            this.armx += 0.7;
        }
        
        if (this.armr > -25 && this.armDir === 0) {
            this.armDir = 1;
        }
        if (this.armr < -35 && this.armDir === 1) {
            this.armDir = 0;    
        }
        if (this.bodyDir === 0) {
            this.y += 1;    
        }
        if (this.bodyDir === 1) {
            this.y -= 1;   
        }
        if (this.bodyDir === 0 && this.y > height - 200) {
            this.bodyDir = 1;    
        }
        if (this.bodyDir === 1 && this.y < 120) {
            this.bodyDir = 0;
        }
        this.count++;
        if (this.count > 40) {
            this.count = 0;
    
            if (this.bossBullets[1].fired === false && this.bossBullets[0].fired === false && this.bossBullets[2].fired === false) {
                this.bossBullets[0].x = this.armx - 20;
                this.bossBullets[0].y = this.y + 105;
                this.bossBullets[0].fired = true;
                this.bossBullets[1].x = this.armx - 20;
                this.bossBullets[1].y = this.y + 105;
                this.bossBullets[1].fired = true;
                this.bossBullets[2].x = this.armx - 20;
                this.bossBullets[2].y = this.y + 105;
                this.bossBullets[2].fired = true;
            }
                
            
        }
       
            this.bossBullets[0].update();
            this.bossBullets[0].x -= 4;
            this.bossBullets[0].y -= 1;
    
            this.bossBullets[1].update();
            this.bossBullets[1].x -= 4;
    
            this.bossBullets[2].update();
            this.bossBullets[2].x -= 4;
            this.bossBullets[2].y += 1;
    }
    if (this.health <= 0) {
        this.show = false;    
    }
};

var level2Boss = function(x, y) {
    this.x = x;
    this.y = y;
    this.health = 30;
    this.count = 0;
    this.show = true;
    this.armDir = 0;
    this.armY = this.y - 15;
    this.armY2 = this.armY - 10;
    this.destX = this.x;
    this.reload = false;
    this.rCount = 0;
    this.bossBullets = [];
    this.ammo = 0;
    for (var i = 0; i < 30; i++) {
        this.bossBullets.push(new enemyBulletObj(20, 200));    
    }
    for (var i = 0; i < 30; i++) {
        this.bossBullets[i].normal = false;    
    }
    this.fCount = 0;
};

level2Boss.prototype.draw = function() {
    if (this.show === true) {
        ellipseMode(CENTER);
        fill(255,215,0);
        stroke(0, 0, 0);
        strokeWeight(2);
        ellipse(this.x, this.y, 90, 30);
        noStroke();
        fill(255, 0, 0);
        ellipse(this.x, this.y, 10, 10);
        fill(255,215,0);
        stroke(0, 0, 0);
        rect(this.x - 40, this.armY, 15, 40);
        rect(this.x + 25, this.armY2 , 15, 40);
        noStroke();
        fill(0, 0, 0);
        ellipse(this.x - 33, this.armY + 41, 16, 10);
        ellipse(this.x + 33,  this.armY2 + 41, 16, 10);
        for (var i = 0; i < this.bossBullets.length; i++) {
            this.bossBullets[i].draw();
            this.bossBullets[i].update();
        }
    }
};

level2Boss.prototype.update = function() {
    if (this.show === true) {
        if (this.reload === false) {
            this.fCount++;
            if (this.armDir === 0)
            {
                this.armY--;
                this.armY2++;
            }
            else if (this.armDir === 1) {
                this.armY++;
                this.armY2--;
            }
            if (this.fCount > 50) {
                this.fCount = 0;
                this.bossBullets[this.ammo].fired = true;
                this.bossBullets[this.ammo].x = this.x - 33;
                this.bossBullets[this.ammo].y = this.armY + 41;
                this.ammo++;
                this.bossBullets[this.ammo].fired = true;
                this.bossBullets[this.ammo].x = this.x + 33;
                this.bossBullets[this.ammo].y = this.armY2 + 41;
                this.ammo++;
                this.bossBullets[this.ammo].fired = true;
                this.bossBullets[this.ammo].x = this.x - 33;
                this.bossBullets[this.ammo].y = this.armY + 41;
                this.ammo++;
                this.bossBullets[this.ammo].fired = true;
                this.bossBullets[this.ammo].x = this.x + 33;
                this.bossBullets[this.ammo].y = this.armY2 + 41;
                this.ammo++;
            }
            if (this.ammo >= 28) {
                this.ammo = 0;    
            }
        }
        for (var i = 0; i < this.bossBullets.length; i++) {
            if (this.bossBullets[i].fired === true) {
                this.bossBullets[i].y += 5;
                if (i !== 0 && i % 3 === 0) {
                    this.bossBullets[i].x += 1.5;    
                }
                else if (i % 1 === 0) {
                    this.bossBullets[i].x -= 1.5;
                }
            }
        }
        if (this.reload === true) {
            this.armY = this.y - 15;
            this.armY2 = this.armY - 10;
        }
        if (this.armDir === 0 && this.armY < this.y - 30) {
            this.armDir = 1; 
        }
        else if (this.armDir === 1 && this.armY > this.y - 10) {
            this.armDir = 0;    
        }
        if (this.x < this.destX) {
            this.x++;    
        }
        else if (this.x > this.destX) {
            this.x--;    
        }
        else if (this.x === this.destX) {
            if (this.reload === false) {
                this.destX = floor(random(100, 500));
                this.count++;
            }
        }
        if (this.count >= 8) {
            this.count = 0;
            this.reload = true;
            this.destX = 500;
        }
        if (this.reload === true && this.x === 500 && this.rCount < 45) {
            if (this.y < 500) {
                this.y++;
            }
            if (this.y >= 500) {
                this.rCount++;    
            }
        }
        if (this.rCount >= 45 && this.reload === true) {
            if (this.y > 100) {
                this.y--;    
            }
            if (this.y === 100) {
                this.reload = false;
                this.count = 0;
                this.rCount = 0;
            }
        }
        
    }
    if (this.health === 0) {
        this.show = false;    
    }
};


var keyPressed2 = function() {
	if (gameState === 2)
	{
	     kyp = true;
	     if (keyCode === UP && player.jump === false && player.onground === true ) 
		{
			player.jump = true;
			player.changeY = -6;
			player.onground = false;
			player.y -= 5;
			if (player.right) {
			    player.changeX = 1.25;
			}
			else {
			   player.changeX = -1.25;    
			}
		}
		if (keyCode === LEFT && player.onground === true ) 
		{
			player.x -= 2;
			player.right = false;
		}
		if (keyCode === RIGHT && player.onground === true ) 
		{
			player.x += 2;    
			player.right = true;
		}
	}
};

keyPressed = function() {
    if (gameState === 2) {
        if (keyCode === 32 && player.ammo < player.bullets.length)
		{
		    player.bullets[player.ammo].fired = true;
            player.bullets[player.ammo].right = player.right;
            if (player.right === true) {
                player.bullets[player.ammo].x = player.x + 30;
            }
            else {
                player.bullets[player.ammo].x = player.x - 10;
            }
            
            player.bullets[player.ammo].y = player.y + 10;
            player.ammo++;
		}
		else if (player.ammo === player.bullets.length && player.bullets[player.bullets.length - 1].used === true && keyCode === 32) 
		{
            player.ammo = 0;
            for (var i = 0; i < player.bullets.length; i++) {
                player.bullets[i].used = false;    
            }
        }
	    
	}
    
};

var displayInfo = function() {
    fill(255, 255, 0);
    textSize(25);
    var gunStatus = player.bullets.length - player.ammo;
    if (gunStatus === 0) {
        gunStatus = "RELOAD";    
    }
    text("Ammo: "+ (gunStatus), 25, 50);
    text("Health:" + " " + player.health, 420, 50);
    
};


var titleScreen = function() {
    titleBackgroundAnimation();
    playButton();
    instrButton();
    title();
};

var level1Obj = function() {
    this.enemies = [];
    this.enemies.push(new enemyObj(500, 400, 2));
    this.enemies.push(new enemyObj(400, 540, 2));
    this.enemies.push(new enemyObj(350, 500, 2));
    this.enemies.push(new enemyObj(150, 260, 2));
    this.enemies[2].health = 2;
    this.enemies[3].health = 2;
    this.checkpoint = 1;
    this.kia = 0;
    this.count = 0;
    this.enemyBullets = [];
    for (var i = 0; i < 10; i++) {
        this.enemyBullets.push(new enemyBulletObj(20, 200));    
    }
    this.ammo = 0;
    this.boss = new level1Boss(400, 120);
};

level1Obj.prototype.draw = function() {
    if (this.checkpoint === 1) {
        fill(0, 0, 0);
        textSize(30);
        text("    LEVEL 1", 200, 120);
        textSize(20);
        fill(255, 255, 0);
        text("GO HERE", 480, 500);
        fill(0, 0, 0);
        noStroke();
        rect(500, 510, 55, 70);
        for (var i = 0; i < 2; i++) {
            this.enemies[i].draw();
        }
    }
    if (this.checkpoint === 2) {
        for (var i = 2; i < this.enemies.length; i++) {
            this.enemies[i].draw();    
        }
        fill(255, 255, 0);
        textSize(15);
        text("GO HERE", 500, 95);
        fill(0, 0, 0);
        rect(500, 100, 80, 60);
    }
    if (this.checkpoint === 3) {
        fill(0, 0, 0);
		textSize(24);
		if (this.boss.show === true) {
		    fill(255, 255, 0);
            text("Boss Health: " + this.boss.health, 220, 50);
            text("Weak Point: Head", 40, 100);
		}
    }
};

level1Obj.prototype.update = function() {
    
    if (this.checkpoint === 1) {
        if (this.enemies[0].show === true) {
            if (this.enemies[0].enemyDir === 0) {
                this.enemies[0].x -= 1;    
            }
            if (this.enemies[0].enemyDir === 1) {
                this.enemies[0].x += 1;    
            }
            if (this.enemies[0].x < 350) {
                this.enemies[0].enemyDir = 1;    
            }
            if (this.enemies[0].x > 520) {
                this.enemies[0].enemyDir = 0;    
            }
        }
        if (this.enemies[1].show === true) {
            if (this.enemies[1].enemyDir === 0) {
                this.enemies[1].x -= 1;    
            }
            if (this.enemies[1].enemyDir === 1) {
                this.enemies[1].x += 1;    
            }
            if (this.enemies[1].x < 300) {
                this.enemies[1].enemyDir = 1;    
            }
            if (this.enemies[1].x > 450) {
                this.enemies[1].enemyDir = 0;    
            }
        }
        if (player.x > 500 && player.x < 560 && player.y > 510 && player.y < 580) {

            currentMap = 1;
            this.checkpoint = 2;
            if (this.enemies[0].show === true) {
                this.enemies[0].show = false;    
            }
            if (this.enemies[1].show === true) {
                this.enemies[1].show = false;    
            }
            player.x = 50;
        }
        
    }
    if (this.checkpoint === 2) {
        for (var i = 0; i < this.enemyBullets.length; i++) {
            this.enemyBullets[i].update();
            this.enemyBullets[i].draw();
        }
        this.count++;
        if (this.enemies[2].show === true) {
            if (this.count > 60) {
                this.enemyBullets[this.ammo].fired = true;
                this.enemyBullets[this.ammo].x = this.enemies[2].x;
                this.enemyBullets[this.ammo].y = this.enemies[2].y;
                this.enemyBullets[this.ammo].right = false;
                this.ammo++;
                
            }
            
        }
        if (this.enemies[3].show === true) {
            if (this.count > 60) {
                this.enemyBullets[this.ammo].fired = true;
                this.enemyBullets[this.ammo].x = this.enemies[3].x + 20;
                this.enemyBullets[this.ammo].y = this.enemies[3].y;
                this.enemyBullets[this.ammo].right = true;
                this.ammo++;
            }
        }
       
        if (this.ammo >= this.enemyBullets.length) {
                this.ammo = 0;    
        }
        if (this.count > 60) {
            this.count = 0;    
        }
        
        if (player.x > 500 && player.x < 580 && player.y > 100 && player.y < 160) {
            currentMap = 2;
            this.checkpoint = 3;
             if (this.enemies[2].show === true) {
                this.enemies[2].show = false;    
            }
            if (this.enemies[3].show === true) {
                this.enemies[3].show = false;    
            }
            player.x = 50;
            player.y = 540;
        }
    }
    if (this.checkpoint === 3) {
        this.boss.draw();
        this.boss.update();
        if (this.boss.health <= 0) {
            gameState = 6;
            player.x= 50;
            player.y = 540;
            player.health = 20;
        }
    }
    
};

level1Obj.prototype.enemyHit = function() {
    var numEnemies = 2;
    var start = 0;
    if (this.checkpoint === 1) {
        numEnemies = 2;    
    }
    else if (this.checkpoint === 2) {
        start = 2;
        numEnemies = 4;    
    }
    else if (this.checkpoint === 3) {
        for (var i = 0; i < player.bullets.length; i++) {
            if (player.bullets[i].x < this.boss.x + 40 && player.bullets[i].x+ 2.5 > this.boss.x - 40 && player.bullets[i].y < this.boss.y + 40 && player.bullets[i].y > this.boss.y - 40 && player.bullets[i].fired === true) {
                this.boss.health--;
                player.bullets[i].fired = false;
                player.bullets[i].used = true;
            }
        }
    }
    if (this.checkpoint === 1 || this.checkpoint === 2) {
        for (var i = start; i < numEnemies; i++) {
            if (this.enemies[i].show === true) {
                var ex = this.enemies[i].x;
                var ey = this.enemies[i].y - 10;
                for (var j = 0; j < player.bullets.length; j++) {
                    if (player.bullets[j].x > this.enemies[i].x && player.bullets[j].x < this.enemies[i].x + 20 && player.bullets[j].y > ey && player.bullets[j].y < this.enemies[i].y + 40 && player.bullets[j].fired === true) {
                        this.enemies[i].health -= 1;
                        if (this.enemies[i].health === 0) {
                            this.enemies[i].show = false;
                            this.kia++;
                        }
                        player.bullets[j].fired = false;
                        player.bullets[j].used = true;
                        
                    }
                }
            }
        }
    }
};

var level2Obj = function() {
    this.enemies = [];
    this.enemies.push(new enemyObj(560, 180, 2));
    this.enemies[this.enemies.length - 1].health = 2;
    this.enemies.push(new enemyObj(20, 180, 2));
    this.enemies[this.enemies.length - 1].health = 2;
    this.enemies.push(new enemyObj(240, 460, 2));
    this.enemies[this.enemies.length - 1].health = 2;
    this.enemies.push(new enemyObj(320, 460, 2));
    this.enemies[this.enemies.length - 1].health = 2;
    player.x = 50;
    player.y = 540;
    this.checkpoint = 1;
    this.count = 0;
    this.enemyBullets = [];
    for (var i = 0; i < 40; i++) {
        this.enemyBullets.push(new enemyBulletObj(20, 200));
    }
    this.enemyammo1 = 0;
    this.enemyammo2 = 5;
    this.enemyammo3 = 10;
    this.enemyammo4 = 25;
    this.enemyDir = 0;
    this.boss = new level2Boss(100, 100);
};

level2Obj.prototype.draw = function() {
    if (this.checkpoint === 1) {
        noStroke();
        fill(226, 245, 17);
        rect(265, 200, 60, 60);
        textSize(18);
        fill(255, 255, 0);
        text("Go Here", 260, 190);
        for (var i = 0; i < 4; i++) {
            this.enemies[i].draw();
        }
    }
    else if (this.checkpoint === 2) {
        noStroke();
        fill(226, 245, 17);
        rect(505, 440, 60, 60);
        textSize(18);
        fill(255, 255, 0);
        text("Go Here", 500, 430);
        for (var i = 0; i < 4; i++) {
            this.enemies[i].draw();
        }
    }
    else if (this.checkpoint === 3) {
        noStroke();
        fill(226, 245, 17);
        rect(60, 520, 60, 60);
        textSize(18);
        fill(255, 255, 0);
        text("Go Here", 55, 510);
        for (var i = 0; i < 4; i++) {
            this.enemies[i].draw();
        }
    }
    else if (this.checkpoint === 4) {
        if (this.boss.health > 0) {
            fill(255, 255, 0);
            textSize(25);
            text("Boss Health: " + this.boss.health, 200, 50);
        }
    }

};

level2Obj.prototype.update = function() {
    this.count++;
    if (this.checkpoint === 1) {
        if (player.x > 265 && player.x < 305 && player.y - 10 < 260 && player.y + 40 > 200) {
            this.checkpoint = 2;
            currentMap = 4;
            for (var i = 0; i < 4; i++) {
                if (i === 0) {
                    this.enemies[i].x = 120;
                    this.enemies[i].y = 540;
                }
                else if (i === 1) {
                    this.enemies[i].y = 540;
                    this.enemies[i].x = 360;
                }
                else if (i === 2) {
                    this.enemies[i].y = 540;     
                }
                else if (i === 3) {
                    this.enemies[i].y = 540;
                    this.enemies[i].x = 460;
                }
                this.enemies[i].show = true;
                this.enemies[i].health = 3;
            }
            player.x = 50;
            player.y = 460;
        }
        for (var i = 0; i < this.enemyBullets.length; i++) {
            if (this.enemyBullets[i].fired === true) {
                if (i < 10) {
                    this.enemyBullets[i].y += 2;
                }
                else if (i >= 10 && i < 25) {
                    this.enemyBullets[i].y += 2.5;    
                }
                else if (i >= 25 && i < 40) {
                    this.enemyBullets[i].y += 2.5;    
                }
            }
        }
        if (this.count > 60) {
            this.count = 0;

            if (this.enemies[2].show === true) {
                if (this.enemyammo1 < 5) {
                    this.enemyBullets[this.enemyammo1].fired = true;
                    this.enemyBullets[this.enemyammo1].x = this.enemies[2].x ;
                    this.enemyBullets[this.enemyammo1].y = this.enemies[2].y;
                    this.enemyBullets[this.enemyammo1].right = false;
                    this.enemyammo1++;
                }
            }
            if (this.enemies[3].show === true) {
                if (this.enemyammo2 < 10) {
                    this.enemyBullets[this.enemyammo2].fired = true;
                    this.enemyBullets[this.enemyammo2].x = this.enemies[3].x + 20;
                    this.enemyBullets[this.enemyammo2].y = this.enemies[3].y;
                    this.enemyBullets[this.enemyammo2].right = true;
                    this.enemyammo2++;    
                }
            }
            if (this.enemies[1].show === true) {
                if (this.enemyammo3 < 25) {
                    this.enemyBullets[this.enemyammo3].fired = true;
                        this.enemyBullets[this.enemyammo3].x = this.enemies[1].x + 20;
                        this.enemyBullets[this.enemyammo3].y = this.enemies[1].y;
                        this.enemyBullets[this.enemyammo3].right = true;
                        this.enemyammo3++;    
                }
            }
            if (this.enemies[0].show === true) {
                 if (this.enemyammo4 < 40) {
                    this.enemyBullets[this.enemyammo4].fired = true;
                        this.enemyBullets[this.enemyammo4].x = this.enemies[0].x;
                        this.enemyBullets[this.enemyammo4].y = this.enemies[0].y;
                        this.enemyBullets[this.enemyammo4].right = false;
                        this.enemyammo4++;    
                }    
            }
        }
        if (this.enemyammo1 >= 5) {
            this.enemyammo1 = 0;    
        }
        if (this.enemyammo2 >= 10) {
            this.enemyammo2 = 5;    
        }
        if (this.enemyammo3 >= 25) {
            this.enemyammo3 = 10;    
        }
        if (this.enemyammo4 >= 40) {
            this.enemyammo4 = 25;    
        }
    }
    else if (this.checkpoint === 2) {
        if (player.x  > 505 && player.x < 565 && player.y - 10 < 500 && player.y + 40 > 440) {
            currentMap = 5;
            this.checkpoint = 3;
            for (var i = 0; i < this.enemyBullets.length; i++) {
                this.enemyBullets[i].fired = false;    
            }
        
            for (var i = 0; i < 4; i++) {
                this.enemies[i].show = false;
                this.enemies[i].health = 3;
                if (i === 0) {
                    this.enemies[i].x = 100;
                    this.enemies[i].y = 100;
                    this.enemies[i].show = true;
                }
                else if (i === 1) {
                    this.enemies[i].x = 200;
                    this.enemies[i].y = 320;
                    this.enemies[i].show = true;
                }
                else if (i === 2) {
                    this.enemies[i].x = 530;   
                    this.enemies[i].show = true;
                }
                
            }
            player.x = 540;
            player.y = 100;
        }
        for (var i = 0; i < this.enemyBullets.length; i++) {
            if (this.enemyBullets[i].fired === true) {
                this.enemyBullets[i].x -= 4;
                this.enemyBullets[i].y -= 3.5;
            }
        }
        if (this.count > 45) {
            
            this.count = 0;
            if (this.enemies[0].show === true) {
                this.enemyBullets[this.enemyammo1].fired = true;
                        this.enemyBullets[this.enemyammo1].x = this.enemies[0].x;
                        this.enemyBullets[this.enemyammo1].y = this.enemies[0].y;
                        this.enemyBullets[this.enemyammo1].right = true;
                        this.enemyammo1++;    
            }
            if (this.enemies[1].show === true) {
                 this.enemyBullets[this.enemyammo2].fired = true;
                        this.enemyBullets[this.enemyammo2].x = this.enemies[1].x;
                        this.enemyBullets[this.enemyammo2].y = this.enemies[1].y;
                        this.enemyBullets[this.enemyammo2].right = true;
                        this.enemyammo2++;    
                
            }
            if (this.enemies[2].show === true) {
                this.enemyBullets[this.enemyammo3].fired = true;
                        this.enemyBullets[this.enemyammo3].x = this.enemies[2].x;
                        this.enemyBullets[this.enemyammo3].y = this.enemies[2].y;
                        this.enemyBullets[this.enemyammo3].right = true;
                        this.enemyammo3++;  
                
            }
            if (this.enemies[3].show === true) {
                this.enemyBullets[this.enemyammo4].fired = true;
                        this.enemyBullets[this.enemyammo4].x = this.enemies[3].x;
                        this.enemyBullets[this.enemyammo4].y = this.enemies[3].y;
                        this.enemyBullets[this.enemyammo4].right = true;
                        this.enemyammo4++;
                
            }
        }
        if (this.enemyammo1 >= 10) {
            this.enemyammo1 = 0;    
        }
        if (this.enemyammo2 >= 20) {
            this.enemyammo2 = 10;    
        }
        if (this.enemyammo3 >= 30) {
            this.enemyammo3 = 20;    
        }
        if (this.enemyammo4 >= 40) {
            this.enemyammo4 = 30;    
        }
       
    }
    else if (this.checkpoint === 3) {
        if (player.x + 20 > 60 && player.x + 20 < 120 && player.y > 520 && player.y < 580) {
            for (var i = 0; i < this.enemies.length; i++) {
                this.enemies[i].show = false;    
            }
            for (var i = 0; i < this.enemyBullets.length; i++) {
                this.enemyBullets[i].fired = false;
            }
            currentMap = 6;
            this.checkpoint = 4;
        }
        if (this.enemyDir === 0) {
            this.enemies[1].x++;
        }
        else {
            this.enemies[1].x--;    
        }
        if (this.enemyDir === 0 && this.enemies[1].x > 430) {
            this.enemyDir = 1;
        }
        else if (this.enemyDir === 1 && this.enemies[1].x < 100){
            this.enemyDir = 0;
        }
        if (this.count > 45) {
            this.count = 0;
            if (this.enemies[0].show === true) {
                this.enemyBullets[this.enemyammo1].fired = true;
                        this.enemyBullets[this.enemyammo1].x = this.enemies[0].x;
                        this.enemyBullets[this.enemyammo1].y = this.enemies[0].y;
                        this.enemyBullets[this.enemyammo1].right = true;
                this.enemyammo1++;
            }
            if (this.enemies[1].show === true) {
                this.enemyBullets[this.enemyammo2].fired = true;
                this.enemyBullets[this.enemyammo2].normal = true;
                this.enemyBullets[this.enemyammo2].x = this.enemies[1].x;
                this.enemyBullets[this.enemyammo2].y = this.enemies[1].y;
                this.enemyBullets[this.enemyammo2].right = true;
                this.enemyammo2++; 
                this.enemyBullets[this.enemyammo2].fired = true;
                this.enemyBullets[this.enemyammo2].normal = true;
                this.enemyBullets[this.enemyammo2].x = this.enemies[1].x;
                this.enemyBullets[this.enemyammo2].y = this.enemies[1].y;
                this.enemyBullets[this.enemyammo2].right = true;
                this.enemyammo2++; 
            }
            if (this.enemies[2].show === true) {
                this.enemyBullets[this.enemyammo3].fired = true;
                this.enemyBullets[this.enemyammo3].normal = false;
                this.enemyBullets[this.enemyammo3].x = this.enemies[2].x;
                this.enemyBullets[this.enemyammo3].y = this.enemies[2].y;
                this.enemyammo3++;
                this.enemyBullets[this.enemyammo3].fired = true;
                this.enemyBullets[this.enemyammo3].normal = false;
                this.enemyBullets[this.enemyammo3].x = this.enemies[2].x;
                this.enemyBullets[this.enemyammo3].y = this.enemies[2].y;
                this.enemyammo3++; 
               
            }
        }
        for (var i = 10; i < 25; i++) {
            if (i % 2 === 1) {
                this.enemyBullets[i].y -= 2.5;    
            }
        }
        for (var i = 25; i < 40; i++) {
            this.enemyBullets[i].y -= 3.5;
            if (i % 3 === 1) {
                this.enemyBullets[i].x -= 0.7;    
            }
        }
        if (this.enemyammo1 >= 10) {
            this.enemyammo1 = 0;    
        }
        if (this.enemyammo2 >= 23) {
            this.enemyammo2 = 10;    
        }
        if (this.enemyammo3 >= 38) {
            this.enemyammo3 = 25;    
        }
       
    }
    else if (this.checkpoint === 4) {
        this.boss.draw();
        this.boss.update();
        if (this.boss.health <= 0) {
            gameState = 6;
            player.x= 50;
            player.y = 540;
            player.health = 20;
        }
    }
    for (var i = 0; i < this.enemyBullets.length; i++) {
        this.enemyBullets[i].update();
        this.enemyBullets[i].draw();
    }
    
};

level2Obj.prototype.enemyHit = function() {
    if (this.checkpoint === 4) {
        for (var i = 0; i < player.bullets.length; i++) {
            if (player.bullets[i].x > this.boss.x - 45 && player.bullets[i].x < this.boss.x + 45 && player.bullets[i].y > this.boss.y - 15 && player.bullets[i].y < this.boss.y + 15 && player.bullets[i].fired === true) {
                this.boss.health--; 
                player.bullets[i].fired = false;
                player.bullets[i].used = true;
            }
        }
    }
    else {
        for (var i = 0; i < 4; i++) {
            if (this.enemies[i].show === true) {
                var ex = this.enemies[i].x;
                var ey = this.enemies[i].y - 10;
                for (var j = 0; j < player.bullets.length; j++) {
                    if (player.bullets[j].x > this.enemies[i].x && player.bullets[j].x < this.enemies[i].x + 20 && player.bullets[j].y > ey && player.bullets[j].y < this.enemies[i].y + 40 && player.bullets[j].fired === true) {
                        this.enemies[i].health -= 1;
                        if (this.enemies[i].health === 0) {
                            this.enemies[i].show = false;
                        }
                        player.bullets[j].fired = false;
                        player.bullets[j].used = true;
                        
                    }
                }
            }
        }
    }
    
};

var star = function() {
    this.x = random(1, 600);
    this.y = random(1, 600);
    this.size = random(1,5);
};

star.prototype.draw = function() {
    noStroke();
    fill(125, 252, 252);
    ellipse(this.x, this.y, this.size, this.size);    
};



var bk = function() {
    this.stars = [];
    for (var i = 0; i < 100; i++) {
        this.stars.push(new star());    
    }
};

bk.prototype.draw = function() {
    background(0,24,72);
    for (var i = 0; i < 100; i++) {
        this.stars[i].draw();    
    }
};

var back = new bk();
var ex = new explosionObj();
var level1 = new level1Obj();
var level2 = new level2Obj();

var game = function() {
    background(255, 255, 255);
    back.draw();
    player.update();
    player.collision();
    player.draw();
    if (player.health <= 0) {
        gameState = 5;
    }
    keyPressed2();
    for (var i = 0; i < player.bullets.length; i++) {
        player.bullets[i].update();
        player.bullets[i].draw();
    }
    if (level === 1) {
        level1.draw();
        level1.update();
        level1.enemyHit();
    }
    else if (level === 2) {
        level2.draw();
        level2.update();
        level2.enemyHit();
    }
    
    
    displayInfo();
    displayTilemap();
    
};

var instructScreen = function() {
    background(135,206,250);
    instructions();
    returnButton();
    var instPlayer = new playerObj(270, 500);
    instPlayer.draw();
    line(200, 520, 250, 520);
    line(230, 500, 250, 520);
    line(230, 540, 250, 520);
    fill(0, 0, 0);
    text("    Your \ncharacter", 50, 500);
    var instEnemy = new enemyObj(520, 500, 3);
    instEnemy.draw();
    fill(0, 0, 0);
    textSize(25);
    text("Enemy", 350, 525);
    line(450, 520, 500, 520);
    line(480, 500, 500, 520);
    line(480, 540, 500, 520);
};

var levelComplete = function() {
    background(83, 196, 71);
    if (level === 2) {
        fill(0, 0, 0);
        textSize(25);
        text("Level Complete", 220, 300);
    }
    nextButton();
    if (currentMap === 2) {
        
        level = 2;
        currentMap = 3;
        level2 = new level2Obj();
    }
    if (currentMap === 6) {
        text("Game Complete", 220, 300);
        level = 3;
    }
};

var gCount = 200;

var gameOver = function() {
    background(gCount, 0, 0);
    if (gCount > 255) {
        gCount = 200;    
    }
    returnButton();
    text("GAME OVER", 200, 300);
};

var restart = function() {
    level = 1;
    currentMap = 0;
    player = new playerObj(110, 100);
    level1 = new level1Obj();
    level2 = new level2Obj();
    for (var i = 0; i < 10; i++) {
        player.bullets[i] = new bulletObj(20, 20);    
    }
};

mouseClicked = function() {
    // Mouse clicks for title screen
	if (gameState === 1) {
	    if (mouseX >= 230 && mouseX <= 380) {
	        if (mouseY >= 320 && mouseY <= 370) {
	            gameState = 2;
	            player.x = 100;
	            player.y = 100;
	        }
	        if (mouseY >= 420 && mouseY <= 470) {
	            gameState = 3;
	        }
	    }
	}
	// Mouse click for instruction screen
	if (gameState === 3) {
	    if (mouseX >= 230 && mouseX <= 380 && mouseY >= 10 && mouseY <= 60) {
	        gameState = 1;    
	    }
	}
	if (gameState === 5) {
	    if (mouseX >= 230 && mouseX <= 380 && mouseY >= 10 && mouseY <= 60) {
	        restart();
	        gameState = 1;    
	    }
	}
	if (gameState === 6) {
	   if (mouseX >= 230 && mouseX <= 380 && mouseY >= 320 && mouseY <= 370) {
	        if (currentMap === 3) {
	            gameState = 2;    
	        }
	        else if (currentMap === 6) {
	            gameState = 1;
	            restart();
	            
	        }
	    }    
	}
  
};

keyReleased = function() {
    keyCode = DOWN;
};

draw = function() {
    if (gameState === 1) {
        titleScreen();
    }
    else if (gameState === 2) {
        game();
    }
    else if (gameState === 3) {
        instructScreen();
    }
    else if (gameState === 5) {
        gameOver();
        gCount++;
        restart();
    }
    else if (gameState === 6) {
        levelComplete();
        
    }
};

}};
