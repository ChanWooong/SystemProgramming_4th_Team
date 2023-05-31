## Grep 명령어를 curse를 이용하여 시각적으로 구현.


### 기존의 grep과의 차별점

기존의 grep 명령어는 Shell에서 모든 처리를 진행.

기능적으로는 좋을지라도 grep의 특성상 시각적인 효과가 부가되는 것이 좋을 것이라 판단.

그러한 판단으로 grep을 screen에 보여줌으로써 시각적인 grep 명령어를 구현.

기본적으로 5줄씩 출력하게 세팅되어있으며, 옵션에 따라 한번에 출력하는 경우도 존재.

------

### 기본 동작 원리

1.KMP 알고리즘을 사용하여 file의 content에서 pattern이 존재하는 부분의 시작 번호를 배열로 main에 호출.

2.window 형태로 screen을 생성하여 UI를 디자인하고, waddstr을 이용하여 데이터를 화면에 출력.

3.각 option 별로 각기 다른 command 수행.


------

### 주요 기능

#### none-option

__command : ./grep (pattern) (filename)__

(filename)에서 (pattern)을 하이라이트로 색칠해주는 기본적인 grep.


#### option -i

__command : ./grep -i (pattern) (filename)__

__main function : 문자열의 대소문자를 구분하지 않고 출력.__

(filename)에서 (pattern)을 찾되, (pattern)이 대문자로 구성된 경우도 같이 하이라이트 해줌.


#### option -c

__command : ./grep -c (pattern) (filename)__

__main function : 문자열이 포함된 라인 개수를 표시.__

(filename)에서 이 포함된 (pattern)줄의 개수를 모두 더하여 출력해주는 시스템.


#### option -n

__command : ./grep -n (pattern) (filename)__

__main function : 일치한 문자열이 포함된 라인 번호를 출력.__

(filename)에서 (pattern)이 포함된 줄의 번호를 순서대로 출력.


#### option -w

__command : ./grep -w (pattern) (filename)__

__main function : 원하는 문자열이 독립된 단어로 존재하는 경우에만 출력.__

(filename) 에서 (pattern)이 하나의 단어로 이루어진 경우에만 출력함.


#### option -v

__command : ./grep -v (pattern) (filename)__

__main function : 입력된 문자열이 포함되지 않는 줄들을 출력하게 한다.__

(filename) 파일에서 (pattern)을 찾아서 해당 구문이 없는 구문들을 출력해줌.


#### option -l

__command : ./grep -l (pattern)__

__main function : 현재 디렉토리에서 해당 문자열을 포함한 파일의 이름만 출력.__

현재 디렉토리를 모두 돌면서 (pattern)을 내용으로 가지는 모든 파일들의 이름을 출력.


#### option -r

__command : ./grep -r (pattern)__

__main function : 서브디렉토리의 파일까지 모두 출력.__

서브 디렉토리까지 (pattern)이 존재하는 모든 파일의 내용을 전부 출력.


#### option -a

__command : ./grep -a (pattern)__

__main function : 해당 디렉토리의 모든 파일에서 원하는 문자열을 출력.__

해당 디렉토리에서 (pattern)이 존재하는 모든 파일의 내용을 전부 출력.


#### option -p

__command : ./grep -p (childProcess) grep (pattern)__

__main function : pipe를 구현한 것으로, child process를 실행시켜 해당 프로세서에 패턴과 부합하는 라인을 모두 출력.__

child process를 실행시켜 pipe를 진행. ( 하이라이트 기능이 포함되어있지 않음. )
