import curses
import time

class ElevatorUI:
    def __init__(self, stdscr):
        self.stdscr = stdscr
        self.init_ui_dimensions()
        self.init_windows()
        self.elevator_floor_count = 10  # 假设有10层楼
        self.elevator_current_floor = self.elevator_floor_count  # 电梯初始在最高层(仅示意)
        
        # 初始化绘制
        self.draw_all_boxes()
        self.draw_elevator_model()
        self.draw_rules_table()
        self.draw_realtime_messages()
        self.draw_passenger_info()

    def init_ui_dimensions(self):
        # 获取当前终端大小
        self.max_y, self.max_x = self.stdscr.getmaxyx()
        
        # 为简单起见，我们假设布局如下：
        # 左上框（电梯模型）：占左半屏宽度和上半屏高度
        self.elevator_box_width = self.max_x // 2
        self.elevator_box_height = self.max_y // 2
        
        # 右上框（实时消息显示区）：占右半屏宽度和上半屏高度
        self.rt_msg_box_width = self.max_x - self.elevator_box_width
        self.rt_msg_box_height = self.elevator_box_height
        
        # 左下框（电梯运行规则执行情况表）：占左半屏宽度和下半屏高度
        self.rules_box_width = self.elevator_box_width
        self.rules_box_height = self.max_y - self.elevator_box_height
        
        # 右下框（乘客信息表）：占右半屏宽度和下半屏高度
        self.passenger_box_width = self.rt_msg_box_width
        self.passenger_box_height = self.rules_box_height
        
        # 定位各框左上角坐标
        self.elevator_box_y, self.elevator_box_x = 0, 0
        self.rt_msg_box_y, self.rt_msg_box_x = 0, self.elevator_box_width
        self.rules_box_y, self.rules_box_x = self.elevator_box_height, 0
        self.passenger_box_y, self.passenger_box_x = self.elevator_box_height, self.elevator_box_width

    def init_windows(self):
        # 创建子窗口（用于绘制框）
        self.elevator_win = curses.newwin(self.elevator_box_height, self.elevator_box_width,
                                          self.elevator_box_y, self.elevator_box_x)
        self.rt_msg_win = curses.newwin(self.rt_msg_box_height, self.rt_msg_box_width,
                                        self.rt_msg_box_y, self.rt_msg_box_x)
        self.rules_win = curses.newwin(self.rules_box_height, self.rules_box_width,
                                       self.rules_box_y, self.rules_box_x)
        self.passenger_win = curses.newwin(self.passenger_box_height, self.passenger_box_width,
                                           self.passenger_box_y, self.passenger_box_x)

    def draw_box(self, win, height, width):
        # 绘制方框
        win.box()
        win.refresh()

    def draw_all_boxes(self):
        # 绘制四个框的外边框
        self.draw_box(self.elevator_win, self.elevator_box_height, self.elevator_box_width)
        self.draw_box(self.rt_msg_win, self.rt_msg_box_height, self.rt_msg_box_width)
        self.draw_box(self.rules_win, self.rules_box_height, self.rules_box_width)
        self.draw_box(self.passenger_win, self.passenger_box_height, self.passenger_box_width)

    def draw_elevator_model(self):
        """
        在左上框绘制电梯模型：
        左侧为电梯井（用竖线表示），右侧为各层楼层指示。
        电梯本体使用"[]"表示，并可在楼层之间移动。
        """
        w = self.elevator_win
        w.clear()
        w.box()
        
        # 电梯井宽度和楼层区域宽度划分(例如左半部为电梯井,右半部为楼层列表)
        shaft_width = self.elevator_box_width // 2
        floor_area_width = self.elevator_box_width - shaft_width - 1
        
        # 绘制电梯井分隔线
        for y in range(1, self.elevator_box_height-1):
            w.addch(y, shaft_width, '|')
        
        # 绘制楼层标记（在右侧区域）
        # 假设电梯从上往下 floor_count层，每层用一行标记
        # 为简单，楼层从上往下递减
        start_y = 1
        available_height = self.elevator_box_height - 2
        # 如果楼层较多，需要适配
        # 假设一行一层
        floors_to_show = min(available_height, self.elevator_floor_count)
        
        for i in range(floors_to_show):
            floor_num = self.elevator_floor_count - i
            w.addstr(start_y + i, shaft_width + 2, f"F{floor_num}")
        
        # 计算电梯实际位置（简单：当前楼层 -> 对应y位置）
        # 当前楼层在楼层列表中的相对位置
        # top floor位置为 start_y (对应最高层)
        # 如果当前楼层在 self.elevator_current_floor, 则其相对位置：
        floor_offset = self.elevator_floor_count - self.elevator_current_floor
        elevator_y = start_y + floor_offset
        if elevator_y < start_y:
            elevator_y = start_y
        elif elevator_y > start_y + floors_to_show - 1:
            elevator_y = start_y + floors_to_show - 1
        
        # 在电梯井画电梯本体: "[]"
        # shaft 左边界为1, shaft_width-1为井内右边界
        # 居中画电梯
        elevator_x = shaft_width//2 - 1
        if elevator_x < 1:
            elevator_x = 1
        w.addstr(elevator_y, elevator_x, "[]")
        
        w.refresh()

    def draw_realtime_messages(self):
        # 在右上框中写入标题等信息
        w = self.rt_msg_win
        w.clear()
        w.box()
        title = "实时消息窗口"
        w.addstr(0, (self.rt_msg_box_width - len(title))//2, title)
        w.refresh()

    def draw_rules_table(self):
        w = self.rules_win
        w.clear()
        w.box()
        title = "电梯运行规则执行情况表"
        w.addstr(0, 1, title)
        w.refresh()

    def draw_passenger_info(self):
        w = self.passenger_win
        w.clear()
        w.box()
        title = "乘客信息表"
        w.addstr(0, 1, title)
        w.refresh()

    def move_elevator(self, target_floor):
        # 修改电梯所在楼层，并重绘电梯模型
        if 1 <= target_floor <= self.elevator_floor_count:
            self.elevator_current_floor = target_floor
            self.draw_elevator_model()

    def update_realtime_messages(self, messages):
        # 在右上框写入多行消息
        w = self.rt_msg_win
        w.clear()
        w.box()
        title = "实时消息窗口"
        w.addstr(0, (self.rt_msg_box_width - len(title))//2, title)
        # 写入消息内容
        for i, msg in enumerate(messages):
            if i+1 < self.rt_msg_box_height-1:
                w.addstr(i+1, 1, msg[:self.rt_msg_box_width-2])
        w.refresh()

    def update_rules_table(self, lines):
        w = self.rules_win
        w.clear()
        w.box()
        title = "电梯运行规则执行情况表"
        w.addstr(0, 1, title)
        # 写入多行数据
        for i, line in enumerate(lines):
            if i+1 < self.rules_box_height-1:
                w.addstr(i+1, 1, line[:self.rules_box_width-2])
        w.refresh()

    def update_passenger_info(self, infos):
        w = self.passenger_win
        w.clear()
        w.box()
        title = "乘客信息表"
        w.addstr(0, 1, title)
        # 写入乘客信息
        for i, info in enumerate(infos):
            if i+1 < self.passenger_box_height-1:
                w.addstr(i+1, 1, info[:self.passenger_box_width-2])
        w.refresh()


def main(stdscr):
    curses.curs_set(0)   # 隐藏光标
    stdscr.clear()
    ui = ElevatorUI(stdscr)
    
    # 测试移动电梯
    floors = [10, 9, 8, 7, 6, 5, 4, 3, 2, 1]
    for f in floors:
        ui.move_elevator(f)
        time.sleep(0.2)
    
    # 测试更新消息
    ui.update_realtime_messages(["系统启动中...", "电梯即将检查故障...", "正常运行"])
    ui.update_rules_table(["规则1：不得超载", "规则2：不得频繁启停"])
    ui.update_passenger_info(["张三, 70kg, 目标楼层: 10", "李四, 60kg, 目标楼层: 8"])
    
    # 等待用户按键退出
    stdscr.getch()

if __name__ == "__main__":
    curses.wrapper(main)