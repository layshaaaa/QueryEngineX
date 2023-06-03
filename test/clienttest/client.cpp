#include <ncurses.h>
#include <string>
#include <vector>

// 假设从服务器获取建议的函数，你需要根据你的服务器接口来实现它
std::vector<std::string> fetch_suggestions_from_server(const std::string& query) {
    return {"suggestion1", "suggestion2", "suggestion3", "suggestion4", "suggestion5"};
}

int main() {
    initscr();  // 初始化ncurses
    cbreak();  // 禁用行缓冲
    noecho();  // 禁止自动显示输入的字符
    keypad(stdscr, TRUE);  // 允许使用特殊键，如方向键和功能键
    
    std::string query;
    std::vector<std::string> suggestions;
    int selected = 0;
    bool select_mode = false;
    
    while (true) {
        clear();  // 清除屏幕
        printw("> %s", query.c_str());  // 显示查询
        if (select_mode) {
            for (int i = 0; i < suggestions.size(); ++i) {
                if (i == selected) {
                    attron(A_REVERSE);  // 反向显示选中的建议
                }
                mvprintw(i + 1, 0, "%s", suggestions[i].c_str());  // 显示建议
                if (i == selected) {
                    attroff(A_REVERSE);  // 关闭反向显示
                }
            }
        }
        refresh();  // 刷新屏幕以显示更新
        
        int ch = getch();  // 获取一个字符
        switch (ch) {
            case '\t':
                if (!select_mode) {
                    suggestions = fetch_suggestions_from_server(query);
                    if (!suggestions.empty()) {
                        selected = 0;
                        select_mode = true;
                    }
                } else {
                    selected = (selected + 1) % suggestions.size();
                }
                break;
            case '\n':
                if (select_mode) {
                    query = suggestions[selected];
                    suggestions.clear();
                    select_mode = false;
                } else {
                    printf("\nprint_search_results\n");
                    char c;
                    scanf("%c", &c);
                    // 在这里处理查询，并打印结果
                    // print_search_results(query);
                }
                break;
            case KEY_BACKSPACE:
                if (!query.empty()) {
                    query.pop_back();
                    select_mode = false;
                    suggestions.clear();
                }
                break;
            default:
                if (ch >= ' ' && ch <= '~') {
                    query.push_back(ch);
                    select_mode = false;
                    suggestions.clear();
                }
                break;
        }
    }
    
    endwin();  // 结束ncurses
    return 0;
}

