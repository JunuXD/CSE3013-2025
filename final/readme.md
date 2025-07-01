# interacting maze in openframeworks for Xcode, OSX

- 서강대학교 컴퓨터공학과 소프트웨어개발도구및환경실습 기말 프로젝트
- 20221559 박준우

# What is this for?

- 맥, XCode에서도 openFrameWorks를 이용한 미로 탐색 알고리즘을 실행 가능하도록 하는 것
- 방금 생성한 미로를 직관적인 인터페이스로 이해할 수 있는 것
- 생성한 미로를 저장해 다시 불러올 수 있게 하는 것

# Requirements

- openframeworks v0.12.0 / v0.12.1
- 오픈프레임웍스 프로젝트 생성기로 생성할 시 ofxGui를 프로젝트의 addon에 반드시 포함시켜야 함.
- 프로젝트 생성기로 생성한 ofApp.h와 ofApp.cpp에 첨부한 같은 파일명의 내용을 복사해 넣어야 함.
- 함께 첨부한 "verdana.ttf" 파일을 프로젝트 앱 폴더/bin/data/ 폴더 내에 위치시켜야 함.

# How-To-Use

- 초기 화면에 진입하면 미로가 정의되어있지 않아 안내 문구가 뜬다.
- 프로젝트 앱 폴더/bin/data/ 폴더 내의 .maz 확장자 파일을 메뉴바에서 "read maze file" 왼쪽의 버튼을 눌러 선택하거나,
- 미로의 행, 열 수를 조정한 후 "generate maze" 왼쪽의 버튼을 눌러 미로를 생성할 수 있다.
- 미로가 화면에 그려졌으면 DFS, BFS 왼쪽의 버튼을 눌러 경로를 확인할 수 있다.
- 생성하거나, 불러온 미로를 "write maze into file" 왼쪽의 버튼을 눌러 .maz 파일로 저장할 수 있다.
- 프로그램을 종료하고 싶다면 "quit" 왼쪽의 버튼을 눌러 종료 가능하다.
