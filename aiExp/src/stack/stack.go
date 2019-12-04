package stack

import (
	"container/list"
	"fmt"
)

//Stack base on List
type Stack struct {
	l *list.List
}

//NewStack 初始化栈
func NewStack() *Stack {
	return &Stack{list.New()}
}

//Push 入栈
func (s *Stack) Push(v interface{}) {
	s.l.PushFront(v)
}

//Pop 出栈
func (s *Stack) Pop() *list.Element {
	e := s.l.Front()
	s.l.Remove(e)
	return e
}

//GetFront 返回栈顶元素
func (s *Stack) GetFront() *list.Element {
	return s.l.Front()
}

//GetFloor 返回栈底元素
func (s *Stack) GetFloor() *list.Element {
	return s.l.Back()
}

//PrintStack 遍历输出栈
func (s *Stack) PrintStack() {
	e := s.l.Front()
	fmt.Print("[")
	for e != nil {
		fmt.Print(e.Value, " ")
		e = e.Next()
	}
	fmt.Println("]")
}

//ClearStack 清空栈
func (s *Stack) ClearStack() {
	s.l.Init()
}

//Len 求栈长
func (s *Stack) Len() int {
	return s.l.Len()
}

//Empty 判断栈是否为空
func (s *Stack) Empty() bool {
	if s.l.Len() == 0 {
		return true
	}
	return false
}
