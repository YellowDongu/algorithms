#pragma once
// 이거 사실 알고리즘 모음집 아님
// 해더에다가 메서드 구현까지 고봉밥으로 꽉꽉 우겨넣었는데
// 클래스 가져다 놓기 편하라고 & 변형하기 편하라고 한곳에 몰아넣은거
// 대부분의 자료들이 편의를 위해 Vector2/Vector3 클래스를 기반으로 사용된거라 주의 필요
// directX와 winAPI, 헤더파일로 나뉘었는데
// d3dx9.h가 필요한 애들과 windows.h가 필요한 애들, 그리고 그 외 나머지로 나눈거


// readMe의 마지막 수정 : 2024-09-27 12:22


// 목록 (헤더 파일 이름)
// Vector2 -> 좌표를 쓰기 편하도록 만들어놓은 클래스 구조체
// Astar => a*알고리즘 길찾기(정수 좌표)
// Collision Simple => 단순 도형(네모와 동그라미) 충돌여부 확인용 - 네모가 회전되있지 않아야함 (AABB 기법)
// CollisionADV -> CollisionSimple에 약간의 기능을 추가한 버젼 => winAPI 전용이 될거 같다
// SAT/SAT3D -> Separating Axis Theorem 알고리즘 충돌여부 확인 (OBB 기법)
// Camera2D -> 2d에서 화면 좌표와 월드 좌표 변환용
// DFSBFS => 노드 컨테이너 관련 노드 탐색 방법 모음
// ImageHandler => bmp 이미지 파일을 불러와서 화면에 렌더링 시켜줌, 2d전용
// ImageRenderer => ImageHandler를 사용해 애니메이션 느낌이 나게 만들어줌
// InputManager => 키보드/마우스 입력 감지(빠른 반응/많은 리소스)
// TimeManager => frame per second같은 프레임 시간 다룸
// NumberOfCases => combination을 이용한 경우의 수 모두 찾기(다만, 정말 모두 탐색하기 때문에 시간복잡도가 지수시간이다(거의 최악))
// Singleton 매크로
// NumberOfCases => 경우의 수를 뽑는거
// DFSBFS => 그래프나 트리같은 비선형 자료구조의 노드탐색(깊이우선/넓이우선 탐색)
// MeshLoader => .x 파일과 .jpg같은 이미지 파일으로 texture, material, mesh 로드
// Camera3D => 3차원 기반 카메라 제어
// UIs => ui 만들때 상속해 쓰려고 한 애들인데 과연 쓸까?
// Vector3 => 좌표를 쓰기 편하도록 만들어놓은 클래스 구조체, D3DXVECTOR3와 호환되게 해놨음