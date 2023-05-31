# Grep 명령어를 curse를 이용하여 시각적으로 구현.

## 기존의 grep과의 차별점
기존의 grep 명령어는 Shell에서 모든 처리를 진행.
기능적으로는 좋을지라도 grep의 특성상 시각적인 효과가 부가되는 것이 좋을 것이라 판단.
그러한 판단으로 grep을 screen에 보여줌으로써 시각적인 grep 명령어를 구현.

기본적으로 5줄씩 출력하게 세팅되어있으며, 옵션에 따라 한번에 출력하는 경우도 존재.

## 주요 기능
### none-option
command : ./grep hi hi.txt
hi.txt 파일에서 hi라는 구문을 하이라이트로 색칠해주는 기본적인 grep.

### option -i
command : ./grep -i hi hi.txt
main function : 문자열의 대소문자를 구분하지 않고 출력.
hi.txt 파일에서 hi라는 구문을 찾되, HI, Hi와 같은 구문도 같이 하이라이트 해줌.

#
