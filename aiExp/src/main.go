package main

import (
	"container/list"
	"fmt"
	"tlib/stack" //stack包在/tack目录下，需自行修改相对路径导入
)

var o1, o2 int = 0, 0

type hanota struct {
	n, pre, done int             //n汉诺塔初始盘子数,pre前一次移动的盘子的位置，初始值为0,done记录汉诺塔已经成功移动的盘子数
	s            [3]*stack.Stack //用栈来存储汉诺塔每个柱子的情况
}

func newHanoi(n int) *hanota { //创建一个汉诺塔盘
	var s [3]*stack.Stack
	s[0] = stack.NewStack()
	for count := n; count > 0; {
		s[0].Push(count)
		count--
	}
	s[1] = stack.NewStack()
	s[2] = stack.NewStack()
	return &hanota{n, -1, 0, s}
}

func (han *hanota) dishToMove() (int, []int) { //求出汉诺塔中下一个需要移动的盘子编号,以及移动的目标个数
	dish := -1
	goal := make([]int, 0)
	for i := 0; i < 3; i++ {
		if i != han.pre && !han.s[i].Empty() {
			for j := 0; j < 3; j++ {
				if j != i && (han.s[j].Empty() || han.s[i].GetFront().Value.(int) < han.s[j].GetFront().Value.(int)) {
					if dish == -1 || (dish != -1 && han.s[dish].GetFront().Value.(int) < han.s[i].GetFront().Value.(int)) {
						dish = i
					}
				}
			}
		}
	}
	if dish != -1 {
		for j := 0; j < 3; j++ {
			if j != dish && (han.s[j].Empty() || han.s[dish].GetFront().Value.(int) < han.s[j].GetFront().Value.(int)) {
				goal = append(goal, j)
			}
		}
	}
	return dish, goal
}

func (han *hanota) printHanoi() { //输出汉诺塔情况
	var e [3]*list.Element
	e[0] = han.s[0].GetFloor()
	e[1] = han.s[1].GetFloor()
	e[2] = han.s[2].GetFloor()
	for i := 0; i < han.n; i++ {
		var ev [3]int
		for j := 0; j < 3; j++ {
			if e[j] == nil {
				ev[j] = 0
			} else {
				ev[j] = e[j].Value.(int)
			}
		}
		defer println(ev[0], ev[1], ev[2])
		if e[0] != nil {
			e[0] = e[0].Prev()
		}
		if e[1] != nil {
			e[1] = e[1].Prev()
		}
		if e[2] != nil {
			e[2] = e[2].Prev()
		}
	}
}

//产生式解法
/*
	在所有盘子中取可移动的最大值的盘子，有序遍历其可以移动的位置，每次将当前最大盘子移动成功后则令层级计数器减一，没有可以移动的盘子时回溯
	层级计数器为1的时候直接将1号盘子移至目标位置，解题成功
*/
func (han *hanota) resolve(hanoiStack *stack.Stack) bool {
	o2++
	/*han.printHanoi()
	fmt.Println()
	*/
	dish, goal := han.dishToMove()
	if han.n == han.done+1 {
		dishNum := han.s[dish].Pop().Value.(int)
		han.s[2].Push(dishNum)
		fmt.Printf("Move dish %d from %d to %d.\n", dishNum, dish+1, 3)
		return true
	}
	/*fmt.Println(dish, goal)
	fmt.Println()
	*/
	if dish == -1 {
		return false
	}
	for _, j := range goal {
		dishNum := han.s[dish].Pop().Value.(int)
		if dishNum == han.n-han.done && j == 2 {
			han.done++
		}
		han.s[j].Push(dishNum)
		fmt.Printf("Move dish %d from %d to %d.\n", dishNum, dish+1, j+1)
		tmp := han.pre
		han.pre = j
		hanoiStack.Push(han)
		if han.resolve(hanoiStack) {
			return true
		}
		fmt.Println("remount")
		hanoiStack.Pop()
		han.pre = tmp
		han.s[dish].Push(han.s[j].Pop().Value)
	}

	return false
}

//递归解法
func (han *hanota) moveDish(level, from, inter, to int) {
	o1++
	if level == 1 {
		han.s[to-1].Push(han.s[from-1].Pop().Value)
		fmt.Printf("Move dish %d from %d to %d.\n", level, from, to)
	} else {
		han.moveDish(level-1, from, to, inter)
		han.s[to-1].Push(han.s[from-1].Pop().Value)
		fmt.Printf("Move dish %d from %d to %d.\n", level, from, to)
		han.moveDish(level-1, inter, from, to)
	}
}

func main() {
	var n int
	fmt.Scanln(&n)

	h1 := newHanoi(n)
	h1.moveDish(n, 1, 2, 3)
	h1.printHanoi() //递归求解汉诺塔问题结

	hanoiStack := stack.NewStack()
	h2 := newHanoi(n)
	hanoiStack.Push(h2)
	fmt.Println(h2.resolve(hanoiStack))
	h2.printHanoi()     //产生式解法求解汉诺塔问题结果
	fmt.Println(o1, o2) //对比两种解法的结果
}
