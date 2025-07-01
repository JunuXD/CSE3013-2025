#pragma once

#include "ofMain.h"
#include "ofxButton.h"
#include "ofxPanel.h"
#include "ofTrueTypeFont.h"

#define WALL_UP 0b1000
#define WALL_DOWN 0b0100
#define WALL_LEFT 0b0010
#define WALL_RIGHT 0b0001
#define WALL_ALL 0b1111
#define WALL_NONE 0b0000

#define MAZE_VERT "|"
#define MAZE_HORIZ "-"
#define MAZE_CORNER "+"
#define MAZE_BLANK " "

typedef pair<int, int> pii;

const int dx[4] = { -1, 1, 0, 0 };
const int dy[4] = { 0, 0, 1, -1 };
const int wall[4] = { WALL_UP, WALL_DOWN, WALL_RIGHT, WALL_LEFT };

class ofApp : public ofBaseApp{

	public:
		void setup() override;
		void update() override;
		void draw() override;
		void exit() override;

		void keyPressed(int key) override;
		void keyReleased(int key) override;
		void mouseMoved(int x, int y ) override;
		void mouseDragged(int x, int y, int button) override;
		void mousePressed(int x, int y, int button) override;
		void mouseReleased(int x, int y, int button) override;
		void mouseScrolled(int x, int y, float scrollX, float scrollY) override;
		void mouseEntered(int x, int y) override;
		void mouseExited(int x, int y) override;
		void windowResized(int w, int h) override;
		void dragEvent(ofDragInfo dragInfo) override;
		void gotMessage(ofMessage msg) override;
	
	// for menu buttons
	// works after maze being ready, doing dfs, bfs works
	// called once - toggle on isd/bfs, twice - toggle off
	void dfsPressed();
	void bfsPressed();
	
	// opens file, if its not .maz, ignores
	void fileOpenPressed();
	
	// saves file if maze generated or loaded
	void fileSavePressed();
	
	// generates maze in maze 2dim vector
	void mazeGenPressed();
	
	// terminal program
	void customExit();
	
	// flag for dfs, bfs, maze load state
	int isDfs, isBfs, mazeState;
	
	// Panel that includes buttons
	ofxPanel gui;
	
	// each button click invokes functions, initiated in setup function
	ofxButton dfsButton, bfsButton, fileOpenButton, fileSaveButton, mazeGenButton, exitButton;
	
	// for printing letters in window
	ofTrueTypeFont font;
	
	// invoked in file open command
	bool processOpenFileSelection(ofFileDialogResult openFileResult);
	
	// maze elements
	vector<vector<char>> maze;
	vector<vector<int>> visited;
	int HEIGHT, WIDTH;
	
	
	// maze creating function
	ofParameter<int> rowInput, colInput;
	
	// returns 0 or 1
	bool getRandBool();
	
	// create maze
	void createMaze(int size_x, int size_y);
	
	// for drawing maze
	float defaultOffsetX, defaultOffsetY, blockWidth;
	
	// draw maze outline
	void drawMazeOutline();
	
	// deletes every maze elements
	void clear();
	
	// for drawing route
	vector<pair<pii, pii>> everyRoute;
	vector<pii> route;
	
	void drawRoute();
	
	// set dfs, bfs routes in route, everyRoute container
	int dfs();
	int bfs();
	
	// for adapting windows size
	float windowWidth, windowHeight;
};
