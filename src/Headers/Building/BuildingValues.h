#pragma once
#define SIN(x) sin(x * 3.141592653589f / 180)
#define COS(x) cos(x * 3.141592653589f / 180)



/*Machinegun:
	- medium health
	- medium range
	- low attack
	- low cost
	- medium/high firerate

Attack behavior:
	the machinegun targets the first closest enemy and a fire bullet 
*/
extern float HEALTHMACHINEGUN;
extern int COSTMACHINEGUN;
extern int ATTACKMACHINEGUN;
extern int RANGEMACHINEGUN;
extern float FIRERATEMACHINEGUN;
#define NAMEMACHINEGUN "Machinegun"


/*Minigun:
	- medium health
	- medium range
	- medium attack
	- medium/high cost
	- high firerate

Attack behavior:
	the minigun targets the first closest enemy and fires bullets with high fire-rate
*/
#define   HEALTHMINIGUN		1500.f
#define     COSTMINIGUN		70
#define	  ATTACKMINIGUN		2
#define	   RANGEMINIGUN		500
#define	FIRERATEMINIGUN		0.05f
#define NAMEMINIGUN "Minigun"




/*Shotgun:
	- medium health
	- short range
	- medium attack
	- medium cost
	- low firerate

Attack behavior:
	the Shotgun aims at the nearest enemy and shoots several bullets in a fan shot
	*/
//#define HEALTHSHOTGUN	1500.f
//#define COSTSHOTGUN		60
//#define ATTACKSHOTGUN	5
//#define RANGESHOTGUN	100
//#define FIRERATESHOTGUN	0.5f
#define NAMESHOTGUN "Shotgun"


/*Rocket:
	- medium health
	- medium range
	- high attack
	- high cost
	- low firerate

Attack behavior:
	Rocket turret targets nearest enemy, then fires a rocket, damaging enemies within the blast radius
*/
//#define HEALTHROCKET		  1500.f
//#define COSTROCKET			  90
//#define	ATTACKROCKET		  12
//#define	RANGEROCKET			  500
//#define	FIRERATEROCKET		  2.f
//#define ROCKETEXPLOSIONRADIUS 100.f
#define NAMEROCKET "Rocket"


/*Sniper:
	- medium health
	- long range
	- high attack
	- high cost
	- low firerate

Attack behavior:
	the sniper targets the farthest enemy then fires a bullet. The bullet damages all enemies it encounters in the way to get to the target.
*/
#define HEALTHSNIPER	1500.f
#define COSTSNIPER		70
#define	ATTACKSNIPER	10
#define	RANGESNIPER		1000
#define	FIRERATESNIPER	0.5f
#define NAMESNIPER "Sniper"


/*
Player Abilities costs
*/
#define COSTBOMB			12
#define COSTTELEPORT		24
#define	COSTEXTRAFIELDS		32
#define	COSTSELL			2