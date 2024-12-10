#include <ncurses.h>
#include <string.h>
#include <unistd.h>

typedef struct {
    int rows;
    int cols;
    int elevator_box_h;
    int elevator_box_w;
    int rt_msg_box_h;
    int rt_msg_box_w;
    int rules_box_h;
    int rules_box_w;
    int passenger_box_h;
    int passenger_box_w;

    int elevator_box_y, elevator_box_x;
    int rt_msg_box_y, rt_msg_box_x;
    int rules_box_y, rules_box_x;
    int passenger_box_y, passenger_box_x;

    WINDOW *elevator_win;
    WINDOW *rt_msg_win;
    WINDOW *rules_win;
    WINDOW *passenger_win;

    int floor_count;
    int current_floor; 
} UIState;

/**
 * 使用 ACS_CKBOARD 绘制边框
 */
void draw_full_block_box(WINDOW *win, int height, int width) {
    wclear(win);
    // 绘制边框：top, bottom, left, right使用 ACS_CKBOARD
    for(int x=0; x<width; x++){
        mvwaddch(win, 0, x, ACS_CKBOARD);
        mvwaddch(win, height-1, x, ACS_CKBOARD);
    }
    for(int y=0; y<height; y++){
        mvwaddch(win, y, 0, ACS_CKBOARD);
        mvwaddch(win, y, width-1, ACS_CKBOARD);
    }
    wrefresh(win);
}

/**
 * 初始化UI尺寸和窗口
 */
void init_ui_dimensions(UIState *ui) {
    getmaxyx(stdscr, ui->rows, ui->cols);
    ui->elevator_box_h = ui->rows / 2;
    ui->elevator_box_w = ui->cols / 2;

    ui->rt_msg_box_h = ui->elevator_box_h;
    ui->rt_msg_box_w = ui->cols - ui->elevator_box_w;

    ui->rules_box_h = ui->rows - ui->elevator_box_h;
    ui->rules_box_w = ui->elevator_box_w;

    ui->passenger_box_h = ui->rules_box_h;
    ui->passenger_box_w = ui->rt_msg_box_w;

    ui->elevator_box_y = 0;
    ui->elevator_box_x = 0;

    ui->rt_msg_box_y = 0;
    ui->rt_msg_box_x = ui->elevator_box_w;

    ui->rules_box_y = ui->elevator_box_h;
    ui->rules_box_x = 0;

    ui->passenger_box_y = ui->elevator_box_h;
    ui->passenger_box_x = ui->elevator_box_w;

    ui->elevator_win = newwin(ui->elevator_box_h, ui->elevator_box_w, ui->elevator_box_y, ui->elevator_box_x);
    ui->rt_msg_win = newwin(ui->rt_msg_box_h, ui->rt_msg_box_w, ui->rt_msg_box_y, ui->rt_msg_box_x);
    ui->rules_win = newwin(ui->rules_box_h, ui->rules_box_w, ui->rules_box_y, ui->rules_box_x);
    ui->passenger_win = newwin(ui->passenger_box_h, ui->passenger_box_w, ui->passenger_box_y, ui->passenger_box_x);

    ui->floor_count = 10;    // 假设10层
    ui->current_floor = ui->floor_count;
}

/**
 * 绘制所有方框外边框(使用█)
 */
void draw_all_boxes(UIState *ui) {
    draw_full_block_box(ui->elevator_win, ui->elevator_box_h, ui->elevator_box_w);
    draw_full_block_box(ui->rt_msg_win, ui->rt_msg_box_h, ui->rt_msg_box_w);
    draw_full_block_box(ui->rules_win, ui->rules_box_h, ui->rules_box_w);
    draw_full_block_box(ui->passenger_win, ui->passenger_box_h, ui->passenger_box_w);
}

/**
 * 在窗口顶部中间打印标题
 */
void print_title(WINDOW *win, int width, const char *title) {
    int startx = (width - (int)strlen(title))/2;
    mvwprintw(win, 0, startx, "%s", title);
    wrefresh(win);
}

/**
 * 绘制电梯模型:
 * - 楼层通过 "█" 行隔开, 每个楼层上下有两行空白, 电梯可以在空白区画出"[]"表示电梯位置。
 * - 左半边为电梯井, 右半边为楼层编号显示(可简化为在右侧打印F+楼层号)
 */
void draw_elevator_model(UIState *ui) {
    WINDOW *w = ui->elevator_win;
    int h = ui->elevator_box_h;
    int wd = ui->elevator_box_w;
    wclear(w);

    // 绘制边框
    for(int x=0; x<wd; x++){
        mvwaddch(w, 0, x, ACS_CKBOARD);
        mvwaddch(w, h-1, x, ACS_CKBOARD);
    }
    for(int y=0; y<h; y++){
        mvwaddch(w, y, 0, ACS_CKBOARD);
        mvwaddch(w, y, wd-1, ACS_CKBOARD);
    }

    // 将电梯井分为两部分，左侧为电梯井，右侧为楼层显示区
    int shaft_width = wd / 2;
    if(shaft_width < 10) shaft_width = wd/2; // 尝试保证一定空间
    if(shaft_width < 3) shaft_width = 3; // 最少宽度
    
    int floor_area_start_x = shaft_width; // 右侧用于打印楼层标记
    
    // 楼层绘制逻辑：
    // 总共 floor_count 层。从顶部向下绘制：
    // 模式（从上到下）： top border 已有
    // floor N boundary (a line of ACS_CKBOARD), then 2 blank lines for that floor
    // floor N-1 boundary ...
    //
    // 计算所需高度：每层3行(1行boundary+2行空白)
    // 总共 floor_count * 3 + 1 (最底部的boundary为下边框)
    // h 应该能容纳这些内容，否则需要缩放
    
    // 计算实际能绘制的楼层数(受窗口高度限制)
    // 每层高度=3行（1行分隔线+2行空白）
    // 总高度 = floor_count*3+1
    // 如果 h < floor_count*3+1 会有截断，这里简单截断显示，不滚动
    int max_floors_can_show = (h-1)/3; // 大约能显示多少层(估计)
    if(max_floors_can_show > ui->floor_count)
        max_floors_can_show = ui->floor_count;

    // 从顶到底绘制楼层线:
    // y=1处开始绘制第一个楼层的顶部boundary line(注意0行是边框)
    int start_y = 1;
    for(int f = ui->floor_count; f > ui->floor_count - max_floors_can_show; f--) {
        // boundary line
        for(int x=1; x<wd-1; x++){
            mvwaddch(w, start_y, x, ACS_CKBOARD);
        }
        // 写楼层号在右侧区域
        // 假设floor_area_start_x+2处打印F楼层号
        // 楼层分隔线画完后，下两行是空白表示此层空间
        mvwprintw(w, start_y, floor_area_start_x+2, "F%d", f);

        // 空白行
        if(start_y+1 < h-1)
            mvwhline(w, start_y+1, 1, ' ', wd-2);
        if(start_y+2 < h-1)
            mvwhline(w, start_y+2, 1, ' ', wd-2);

        start_y += 3;
        if(start_y >= h-1) break;
    }

    // 最后在已绘制范围内，可能需要在最后补充底线(但已用下边框代替)
    // 电梯位置计算：
    // current_floor 楼层在列表中的相对位置：
    // 顶层为 floor_count，对应第一个楼层块的上方（不计初始上边框）
    // 若当前楼层=ui->current_floor，则在相应的空白区域中绘制电梯"[]"
    // 我们需要计算该楼层在可显示楼层列表中的相对索引。
    
    // 楼层显示是从 floor_count 向下显示 max_floors_can_show 层
    // 顶层 f = floor_count 显示在最上方
    // current_floor = floor_count 时，它是列表中第0层，y起点=1+1(空白区域第一行) = 2行
    // 对于 f 层， boundary在 start_of_this_floor，电梯应在 boundary+1或+2 行
    // 为了简单起见，我们再计算：
    int displayed_top_floor = ui->floor_count;
    int displayed_bottom_floor = ui->floor_count - max_floors_can_show + 1;
    // 如果 current_floor 不在显示范围内，则电梯不可见
    if(ui->current_floor <= displayed_top_floor && ui->current_floor >= displayed_bottom_floor) {
        int floor_index_from_top = displayed_top_floor - ui->current_floor;
        // 楼层 boundary 行位置：1 + floor_index_from_top*3
        int floor_boundary_y = 1 + floor_index_from_top * 3;
        // 电梯应画在 boundary line下的两行空白区中间行
        int elevator_y = floor_boundary_y + 1; 
        int elevator_x = shaft_width/2 - 1;
        if(elevator_x < 1) elevator_x = 1;
        // 电梯用 "[]"表示
        mvwprintw(w, elevator_y, elevator_x, "[]");
    }

    wrefresh(w);
}

/**
 * 更新右上区域（实时消息）
 */
void update_realtime_messages(UIState *ui, const char **messages, int count) {
    WINDOW *w = ui->rt_msg_win;
    int h = ui->rt_msg_box_h;
    int wd = ui->rt_msg_box_w;
    wclear(w);

    // 画边框
    for(int x=0; x<wd; x++){
        mvwaddch(w, 0, x, ACS_CKBOARD);
        mvwaddch(w, h-1, x, ACS_CKBOARD);
    }
    for(int y=0; y<h; y++){
        mvwaddch(w, y, 0, ACS_CKBOARD);
        mvwaddch(w, y, wd-1, ACS_CKBOARD);
    }

    print_title(w, wd, "实时消息窗口");

    for(int i=0; i<count && i<h-2; i++){
        mvwprintw(w, i+1, 1, "%s", messages[i]);
    }
    wrefresh(w);
}

/**
 * 更新左下区域（规则表）
 */
void update_rules_table(UIState *ui, const char **lines, int count) {
    WINDOW *w = ui->rules_win;
    int h = ui->rules_box_h;
    int wd = ui->rules_box_w;
    wclear(w);

    // 边框
    for(int x=0; x<wd; x++){
        mvwaddch(w, 0, x, ACS_CKBOARD);
        mvwaddch(w, h-1, x, ACS_CKBOARD);
    }
    for(int y=0; y<h; y++){
        mvwaddch(w, y, 0, ACS_CKBOARD);
        mvwaddch(w, y, wd-1, ACS_CKBOARD);
    }

    print_title(w, wd, "电梯运行规则执行情况表");
    for(int i=0; i<count && i<h-2; i++){
        mvwprintw(w, i+1, 1, "%s", lines[i]);
    }
    wrefresh(w);
}

/**
 * 更新右下区域（乘客信息表）
 */
void update_passenger_info(UIState *ui, const char **infos, int count) {
    WINDOW *w = ui->passenger_win;
    int h = ui->passenger_box_h;
    int wd = ui->passenger_box_w;
    wclear(w);

    // 边框
    for(int x=0; x<wd; x++){
        mvwaddch(w, 0, x, ACS_CKBOARD);
        mvwaddch(w, h-1, x, ACS_CKBOARD);
    }
    for(int y=0; y<h; y++){
        mvwaddch(w, y, 0, ACS_CKBOARD);
        mvwaddch(w, y, wd-1, ACS_CKBOARD);
    }

    print_title(w, wd, "乘客信息表");
    for(int i=0; i<count && i<h-2; i++){
        mvwprintw(w, i+1, 1, "%s", infos[i]);
    }
    wrefresh(w);
}

/**
 * 移动电梯到指定楼层，并刷新显示
 */
void move_elevator(UIState *ui, int target_floor) {
    if(target_floor >=1 && target_floor <= ui->floor_count) {
        ui->current_floor = target_floor;
        draw_elevator_model(ui);
    }
}

int main() {
    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);

    UIState ui;
    init_ui_dimensions(&ui);

    draw_all_boxes(&ui);
    draw_elevator_model(&ui);
    print_title(ui.rt_msg_win, ui.rt_msg_box_w, "实时消息窗口");
    print_title(ui.rules_win, ui.rules_box_w, "电梯运行规则执行情况表");
    print_title(ui.passenger_win, ui.passenger_box_w, "乘客信息表");

    // 测试电梯移动
    for(int f=ui.floor_count; f>=1; f--){
        move_elevator(&ui, f);
        usleep(200000);
    }

    const char *msgs[] = {"系统已启动", "等待电梯指令", "电梯正常运行"};
    update_realtime_messages(&ui, msgs, 3);

    const char *rules[] = {"规则1: 不得超载", "规则2: 不得频繁启停"};
    update_rules_table(&ui, rules, 2);

    const char *passengers[] = {"张三,70kg,目标10F", "李四,60kg,目标8F"};
    update_passenger_info(&ui, passengers, 2);

    // 等待按键退出
    getch();
    endwin();
    return 0;
}