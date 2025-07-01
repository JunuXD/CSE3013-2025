#include "ofApp.h"

// init everything
void ofApp::setup(){
	
	// init menu
	gui.setup();
	gui.add(dfsButton.setup("dfs"));
	gui.add(bfsButton.setup("bfs"));
	gui.add(fileOpenButton.setup("read maze file"));
	gui.add(fileSaveButton.setup("write maze into file"));
	gui.add(mazeGenButton.setup("generate maze"));
	gui.add(rowInput.set("row of gen.maze", 10, 1, 30));
	gui.add(colInput.set("col of gen.maze", 10, 1, 30));
	gui.add(exitButton.setup("quit"));
	
	// each function will be called on button click
	dfsButton.addListener(this, &ofApp::dfsPressed);
	bfsButton.addListener(this, &ofApp::bfsPressed);
	fileOpenButton.addListener(this, &ofApp::fileOpenPressed);
	mazeGenButton.addListener(this, &ofApp::mazeGenPressed);
	fileSaveButton.addListener(this, &ofApp::fileSavePressed);
	exitButton.addListener(this, &ofApp::customExit);
	
	// init flags - all false
	isDfs = 0;
	isBfs = 0;
	mazeState = 0;
	
	// init string writer
	font.load("verdana.ttf", 32, true, true);
	
	// init window Width, Height
	windowWidth = ofGetWidth();
	windowHeight = ofGetHeight();
	
	// set background colour to white
	ofSetBackgroundColor(ofColor::white);
}

//--------------------------------------------------------------
void ofApp::update(){
	// nothing to update, updates are done in events
}

// draw black outline of Maze
void ofApp::drawMazeOutline(){
	// outline colour : black / thickness : 2
	ofSetColor(ofColor::black);
	ofSetLineWidth(2);
	
	// draw 4-dir walls, according to maze array
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			ofPoint left_up = ofPoint(defaultOffsetX + j * blockWidth,
									  defaultOffsetY + i * blockWidth);
			ofPoint left_down = ofPoint(defaultOffsetX + j * blockWidth,
										defaultOffsetY + (i + 1) * blockWidth);
			ofPoint right_up = ofPoint(defaultOffsetX + (j + 1) * blockWidth,
									   defaultOffsetY + i * blockWidth);
			ofPoint right_down = ofPoint(defaultOffsetX + (j + 1) * blockWidth,
										 defaultOffsetY + (i + 1) * blockWidth);

			if (maze[i][j] & WALL_UP) ofDrawLine(left_up, right_up);
			if (maze[i][j] & WALL_DOWN) ofDrawLine(left_down, right_down);
			if (maze[i][j] & WALL_LEFT) ofDrawLine(left_up, left_down);
			if (maze[i][j] & WALL_RIGHT) ofDrawLine(right_up, right_down);
		}
	}
}

// draw function, called in every frame
void ofApp::draw(){
	// draw gui (menu bar)
	gui.draw();
	
	// text colour : black
	ofSetColor(ofColor::black);
	
	// if maze is not there, write a instructions
	if(!mazeState){
		font.drawString("you can make maze or load maze file!", windowWidth / 2 - 400, windowHeight / 2);
	}
	
	// if maze is there, draw a maze;
	else {
		drawMazeOutline();
		
		// draw dfs, bfs routes;
		if(isDfs){
			font.drawString("DFS Mode", windowWidth / 2 - 110, 40);
		}
		if(isBfs){
			font.drawString("BFS Mode", windowWidth / 2 - 110, 40);
		}
		
		// call drawRoute function
		// it will check itself - does dfs or bfs mode turned on
		drawRoute();
	}
}

//--------------------------------------------------------------
void ofApp::exit(){
	
}

// function called by quit button
void ofApp::customExit(){
	// leave a log
	ofLog() << "Closing application via request of user";
	// exit program
	ofExit();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

// update blockWidth and Offset when window size changed
void ofApp::windowResized(int w, int h){
	windowWidth = w;
	windowHeight = h;
	
	blockWidth = MIN((windowWidth - 10) / WIDTH, (windowHeight - 60) / HEIGHT);
	
	defaultOffsetX = (windowWidth - (blockWidth * WIDTH)) / 2;
	defaultOffsetY = (windowHeight - (blockWidth * HEIGHT)) / 2 + 15;
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

// function that include bfs algorithm - using queue
int ofApp::bfs(){
	// init queue
	std::queue<pii> q;
	
	// 2dim vector for visited - will be distance from start
	visited = vector<vector<int>> (HEIGHT, vector<int> (WIDTH, -1));
	
	// push starting point, check starting point as visited
	q.push({0, 0});
	visited[0][0] = 1;
	// target is most right-down cell
	pii target = {HEIGHT - 1, WIDTH - 1};
	
	// bfs main loop, considering each cell at-most once
	while(!q.empty()){
		pii curr = q.front();
		
		// if we found target, we dont need to search more
		if(curr == target) break;
		
		// add possible direction's cell in queue, and check it as visited
		for (int k = 0; k < 4; k++) {
			// now considering next cell's position
			int nx = curr.first + dx[k], ny = curr.second + dy[k];

			// if out of range - continue
			if (nx < 0 || nx >= HEIGHT || ny < 0 || ny >= WIDTH) continue;

			// there is no wall between cur - next cell && next cell not visited
			if (!(maze[curr.first][curr.second] & wall[k]) && visited[nx][ny] < 0) {
				// add on queue
				q.push({ nx, ny });
				// check visited as distance
				visited[nx][ny] = visited[curr.first][curr.second] + 1;

				// add current vertex in everyRoute
				everyRoute.push_back({curr, pii{nx, ny}});
			}
		}

		// pop
		q.pop();
	}
	
	// back-track shortest path by following back from end-point
	pii curr = {HEIGHT - 1, WIDTH - 1};
	target = {0, 0};
	
	while (true) {
		// cells in this loop is included in shortest path
		route.push_back(curr);

		if (curr == target) break;

		for (int k = 0; k < 4; k++) {
			int nx = curr.first + dx[k], ny = curr.second + dy[k];

			if (nx < 0 || nx >= HEIGHT || ny < 0 || ny >= WIDTH) continue;

			if (!(maze[curr.first][curr.second] & wall[k])
				&& (visited[nx][ny] == visited[curr.first][curr.second] - 1)){
				curr = { nx, ny };

				break;
			}
		}
	}
	
	// clear visited 2dim vector used here.
	visited.clear();
	
	// return 1 to make isBfs true
	return 1;
}

// function that called by bfs button
void ofApp::bfsPressed(){
	ofLog() << "bfs button was pressed!";
	
	// if maze is not loaded, we cannot operate bfs
	if(!mazeState){
		return;
	}
	
	// if there is prev route, toggle off bfs, and just delete route
	if(isBfs){
		isBfs = 0;
		
		everyRoute.clear();
		route.clear();
		
		return;
	}
	// if there is dfs route, delete this path and go on
	else if(isDfs){
		isDfs = 0;
		
		everyRoute.clear();
		route.clear();
	}
	
	// if bfs ends well, set isBfs to 1
	isBfs = bfs();
}

// function that include iterative dfs algorithm - using stack
int ofApp::dfs(){
	std::stack<pii> s;
	
	// init visited 2dim vector to 0
	visited = vector<vector<int>> (HEIGHT, vector<int> (WIDTH, 0));
	
	// starts in 0, 0
	s.push({0, 0});
	visited[0][0] = 1;
	pii target = {HEIGHT - 1, WIDTH - 1};
	
	
	while(!s.empty()){
		pii curr = s.top();
		
		if(curr == target){
			// values in stack when curr == target is shortest path
			// save all
			while(!s.empty()){
				route.push_back(s.top());
				s.pop();
			}
			
			break;
		}
		
		bool endflag = true;

		for (int k = 0; k < 4; k++) {
			int nx = curr.first + dx[k], ny = curr.second + dy[k];

			if (nx < 0 || nx >= HEIGHT || ny < 0 || ny >= WIDTH) continue;

			if (!(maze[curr.first][curr.second] & wall[k])
				&& visited[nx][ny] == 0) {
				s.push({ nx, ny });
				everyRoute.push_back({ curr, { nx, ny } });
				visited[nx][ny] = true;
				endflag = false;
				// if it moves to one direction, go there instantly by breaking here
				break;
			}
		}

		if (endflag) s.pop();
	}
	
	// clears visited 2dim vector here.
	visited.clear();
	
	return 1;
}

void ofApp::dfsPressed(){
	ofLog() << "dfs button was pressed!";
	
	// if maze is not loaded, we cannot operate dfs
	if(!mazeState){
		return;
	}
	
	// if there is prev route, toggle off dfs, and just delete route
	if(isDfs){
		isDfs = 0;
				
		everyRoute.clear();
		route.clear();

		return;
	}
	// if there is bfs route, delete this path and go on
	else if (isBfs){
		isBfs = 0;
		
		everyRoute.clear();
		route.clear();
	}
	
	isDfs = dfs();
}

// file open button function
void ofApp::fileOpenPressed(){
	ofLog() << "file open button was pressed!";
	
	// if there is already a maze, just delete it and end it
	if(mazeState){
		clear();
	}
	// if there is no maze, open a file
	// if success to open file, process open file selection
	else {
		ofFileDialogResult openFileResult= ofSystemLoadDialog("Select a only .maz file");
		
		if(openFileResult.bSuccess){
			ofLogVerbose("User selected a file");
			
			//We have a file, check it and process it
			if(!processOpenFileSelection(openFileResult)){
				mazeState = 1;
			} else {
				ofLog() << "error occured while processing Openfile";
			}
			
		}
	}
}

// if file open success
bool ofApp::processOpenFileSelection(ofFileDialogResult openFileResult){

	string fileName = openFileResult.getName();
	string filePath = openFileResult.getPath();
	
	
	// checks if it is '.maz' extension
	size_t pos;
	pos = filePath.find_last_of(".");
	
	// returns after writing log if file is not .maz extension
	if((pos == string::npos) || (pos == 0) || !(filePath.substr(pos + 1) == "maz")){
		ofLog() << "needs a '.maz extension";
		return 1;
	}
	
	ofFile file(fileName);
	
	if( !file.exists()){
		ofLog() << "Target file does not exists.";
		
		return 1;
	}
	else{
		ofLog() << "We found the target file.";
	}
	
	// loads file into buffer
	ofBuffer buffer(file);

	// put maze file to 2dim vector
	vector<string> lines;
	for (auto l : buffer.getLines()) {
		lines.push_back(l);
	}
	
	// decides Height, Width by size of maze file
	HEIGHT = lines.size() / 2;
	WIDTH = lines[0].size() / 2;
	
	maze = vector<vector<char>> (HEIGHT, vector<char> (WIDTH, 0));
	
	// for each cell of maze, put Walls via input file
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			int x = 1 + 2 * i;
			int y = 1 + 2 * j;

			if (lines[x - 1][y] == '-') maze[i][j] |= WALL_UP;
			if (lines[x + 1][y] == '-') maze[i][j] |= WALL_DOWN;
			if (lines[x][y - 1] == '|') maze[i][j] |= WALL_LEFT;
			if (lines[x][y + 1] == '|') maze[i][j] |= WALL_RIGHT;
		}
	}
	
	// decides blockWidth, Offsets by some math
	blockWidth = MIN((windowWidth - 10) / WIDTH, (windowHeight - 60) / HEIGHT);
	
	defaultOffsetX = (windowWidth - (blockWidth * WIDTH)) / 2;
	defaultOffsetY = (windowHeight - (blockWidth * HEIGHT)) / 2 + 15;
	
	return 0;
}

void ofApp::fileSavePressed(){
	
	// if there is no maze, nothing to save
	if(!mazeState){
		ofLog() << "There is no maze to save";
		
		return;
	}
	
	// decide a file name by current time
	std::string timestamp = ofGetTimestampString("%Y-%m-%d-%H-%M-%S");
	std::string filename = "output_" + timestamp + ".maz";
	
	ofFile file;
	
	// open file and write maze
	if(file.open(filename, ofFile::WriteOnly)){
		ofBuffer buffer;
		
		for(int i = 0; i < HEIGHT; i++){
			for(int j = 0; j < WIDTH; j++){
				buffer.append(MAZE_CORNER);
				buffer.append((maze[i][j] & WALL_UP)? MAZE_HORIZ : MAZE_BLANK);
				if(j == WIDTH - 1) buffer.append(MAZE_CORNER);
			}
			buffer.append("\n");
			for(int j = 0; j < WIDTH; j++){
				buffer.append((maze[i][j] & WALL_LEFT)? MAZE_VERT : MAZE_BLANK);
				buffer.append(MAZE_BLANK);
				if(j == WIDTH - 1) buffer.append(MAZE_VERT);
			}
			buffer.append("\n");
			
			if(i == HEIGHT - 1){
				for(int j = 0 ; j < WIDTH; j++){
					buffer.append(MAZE_CORNER);
					buffer.append(MAZE_HORIZ);
					
					if(j == WIDTH - 1) buffer.append(MAZE_CORNER);
				}
			}
		}
		
		file.writeFromBuffer(buffer);
		
		file.close();
		
		ofLog() << "Data saved to " << filename;
	}
	// if file open fails, write a log.
	else {
		ofLogError() << "Could not open file for write";
	}
}

// clearing maze info
void ofApp::clear(){
	maze.clear();
	visited.clear();
	HEIGHT = WIDTH = 0;
	mazeState = 0;
	isDfs = isBfs = 0;
	route.clear();
	everyRoute.clear();
}

// maze gen button function
void ofApp::mazeGenPressed(){
	
	// if there is maze already, clear it and go on
	if(mazeState){
		clear();
	}
	
	// calls a createMaze function by menu bar values
	createMaze(colInput, rowInput);
	
	mazeState = 1;
	
	// decides width, offsets
	blockWidth = MIN((windowWidth - 10) / WIDTH, (windowHeight - 60) / HEIGHT);
	
	defaultOffsetX = (windowWidth - (blockWidth * WIDTH)) / 2;
	defaultOffsetY = (windowHeight - (blockWidth * HEIGHT)) / 2 + 15;
}

bool ofApp::getRandBool(){
	return rand() & 1;
}

// maze gen lgorithm - eller's algorithm
void ofApp::createMaze(int M, int N){
	int groupNum = 0;
	HEIGHT = N;
	WIDTH = M;
	
	maze = vector<vector<char>> (N, vector<char> (M, WALL_ALL));
	vector<vector<int>> group = vector<vector<int>> (N, vector<int> (M, 0));;
	
	for(int i = 0; i < N; i++){
		// give group number to undefined ones
		for(int j = 0; j < M; j++){
			if(group[i][j] > 0) continue;
			
			group[i][j] = ++groupNum;
		}
		
		for(int j = 0; j < M - 1; j++){
			if(group[i][j] == group[i][j+1]) continue;
			
			if(i == N - 1 || getRandBool()){
				int changed = MAX(group[i][j], group[i][j+1]);
				int changeto = MIN(group[i][j], group[i][j+1]);
				
				for(int k = 0; k < M; k++){
					if(group[i][k] != changed) continue;
					
					group[i][k] = changeto;
				}
				
				maze[i][j] ^= WALL_RIGHT;
				maze[i][j + 1] ^= WALL_LEFT;
			}
		}
		
		if(i == N - 1) break;
		
		for(int j = 0; j < M; j++){
			if(group[i][j] < 0) continue;
			
			int cur = group[i][j], count = 1, mand;
			
			for(int k = j + 1; k < M; k++){
				if(group[i][k] == cur) count++;
			}
			
			mand = rand() % count;
			
			for(int k = j, cnt = 0; k < M; k++){
				if(group[i][k] != cur) continue;
				
				if(cnt++ == mand || getRandBool()){
					group[i + 1][k] = group[i][k];
					
					maze[i][k] ^= WALL_DOWN;
					maze[i+1][k] ^= WALL_UP;
				}
				
				group[i][k] *= -1;
			}
		}
	}
	
	group.clear();
}

void ofApp::drawRoute(){
	if(isDfs || isBfs){
		// draw every route used while search
		ofSetColor(ofColor::red);
		ofSetLineWidth(2);
		
		for (pair<pii, pii> x : everyRoute) {
			ofPoint p1 = ofPoint(defaultOffsetX + x.first.second * blockWidth + blockWidth / 2,
								 defaultOffsetY + x.first.first * blockWidth + blockWidth / 2);
			ofPoint p2 = ofPoint(defaultOffsetX + x.second.second * blockWidth + blockWidth / 2,
								 defaultOffsetY + x.second.first * blockWidth + blockWidth / 2);

			ofDrawLine(p1, p2);
		}

		int maxx = (int)(route.size());

		// draw route in shortest path
		ofSetColor(ofColor::green);
		for (int i = 1; i < maxx; i++) {
			ofPoint p1 = ofPoint(defaultOffsetX + route[i].second * blockWidth + blockWidth / 2,
								 defaultOffsetY + route[i].first * blockWidth + blockWidth / 2);
			ofPoint p2 = ofPoint(defaultOffsetX + route[i - 1].second * blockWidth + blockWidth / 2,
								 defaultOffsetY + route[i - 1].first * blockWidth + blockWidth / 2);

			ofDrawLine(p1, p2);
		}
	}
}
