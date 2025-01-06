#include <iostream>
#include <windows.h>

using namespace std;

int board[4][4] = {
    {0, 0, 1, 0},
    {0, 4, 0, 0},
    {0, 0, 2, 0},
    {0, 1, 0, 0},
};

bool editable[4][4];//Check if the cell is editable

bool error[4][4]={{1,1,1,1},{1,1,1,1},{1,1,1,1},{1,1,1,1}};//Check if the line or block has conflict number

bool finished[4][4]={{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};//Check if the line or block is finished

bool conflict_r, conflict_c, conflict_block;//Check if the line or block has conflict number

bool finished_r, finished_c, finished_block;//Check if the line or block is finished

int cur_r = 0, cur_c = 0;//The current position

void check_horizontal(int i)
{ 	
	conflict_r = 0;
	//Check if the row has conflict number
	for(int a = 0; a < 4; ++a ){
    	for(int b = a+1; b < 4; ++b ){
    		if((board[i][a] != 0) && (board[i][a] == board[i][b])){	//If two cells have the same number except for 0
    				conflict_r = 1;
    				for(int m = 0; m < 4; ++m){
    				error[i][m]=0;									//The horizontal line has conflict number
    				} 
    				break;
				}

		}
	}
	if (!conflict_r) //If the row has no conflict number
    {
        for (int m = 0; m < 4; ++m)
        {
            error[i][m] = 1; // Reset the state of this row
        }
    }
    finished_r = 0;
	//Check if the row is finished
	for(int a = 0; a < 4; ++a){
        if(board[i][a] == 0){ // If there is any 0 in the row
            finished_r = 0;    // Not finished
        }
    }
	for(int a = 0; a < 4; ++a ){
    	for(int b = a+1; b < 4; ++b ){
    		if((board[i][a] != board[i][b])){	//Check the cell isn't 0 and each cell has different number
    				finished_r=1;
				}
    }
	}
	
	if(finished_r){
		for(int m=0; m<4; ++m){
			finished[i][m]=1;
		}
	}
	
}

    /* TODO: Check if a horizontal line has conflict number, or is finished. */


void check_vertical(int i)
{
	conflict_c = 0;//Check if has conflict number
	//Check if the column has conflict number
	for(int a = 0; a < 4; ++a ){							
    	for(int b = a+1; b < 4; ++b ){
    		if((board[a][i] != 0) && (board[a][i] == board[b][i])){	//If two cells have the same number except for 0
    			conflict_c = 1;
    			for(int m = 0; m < 4; ++m){
    				error[m][i]=0;								//The vertical line has conflict number
    			}
    			break;
			}
		}
	}
	if (!conflict_c) //If the column has no conflict number
    {
        for (int m = 0; m < 4; ++m)
        {
            error[m][i] = 1; // Reset the state of this column
        }
    }
	
}
	
    /* TODO: Check if a vertical line has conflict number, or is finished. */
    			
void check_block(int x, int y)
{
    int start_r, end_r, start_c, end_c;
    bool conflict_block = 0;//check if the block has conflict number

    //mark the range of the current block
    switch(x) {
    case 0: start_r = 0, end_r = 1; break;
    case 1: start_r = 0, end_r = 1; break;
    case 2: start_r = 2, end_r = 3; break;
    case 3: start_r = 2, end_r = 3; break;
    }
    switch(y) {
    case 0: start_c = 0, end_c = 1; break;
    case 1: start_c = 0, end_c = 1; break;
    case 2: start_c = 2, end_c = 3; break;
    case 3: start_c = 2, end_c = 3; break;
    }

    for(int a = start_r; a <= end_r; ++a) {
        for(int b = start_c; b <= end_c; ++b) {
            if(board[a][b] != 0) {	// while current cell isn't 0
                if(b + 1 <= end_c && board[a][b] == board[a][b+1]) {	//check the right cell
                    conflict_block = 1;
                }
                if(a + 1 <= end_r && board[a][b] == board[a+1][b]) {	//check the cell below
                    conflict_block = 1;
                }
                if(a + 1 <= end_r && b + 1 <= end_c && board[a][b] == board[a+1][b+1]) {	//check the right cell below
                    conflict_block = 1;
                }
            }
        }
    }
    if (conflict_block) {//If the row has conflict number
        for(int a = start_r; a <= end_r; ++a) {
            for(int b = start_c; b <= end_c; ++b) {
                error[a][b] = 0;
            }
        }
    }
   
    if (!conflict_block) {//If the row has no conflict number
        for(int a = start_r; a <= end_r; ++a) {
            for(int b = start_c; b <= end_c; ++b) {
                error[a][b] = 1; //Reset the state of this block
            }
        }
        check_horizontal(cur_r);
		check_vertical(cur_c); //check again the lines are in the correct state
    }

}
    /* TODO: Check if a block has conflict number, or is finished. */


void fill_number(int c)
{
    /* TODO: Fill a number in to the cell the cursor is now pointing to.
             After fill in the number, check the horizontal line, the
             vertical line and the block the cell is in.
     */
	board[cur_r][cur_c]=c-48;	//fill in number
	check_horizontal(cur_r);
	check_vertical(cur_c);
	check_block(cur_r,cur_c);
	
}

int move_cursorR(char c)
{
	if((c == 'W' || c == 'w') && cur_r > 0){	//Check if in a valid range
		if(editable[cur_r-1][cur_c]==1) cur_r -= 1;	//If the position above doesn't have number
		else if(editable[cur_r-1][cur_c]==0 && cur_r - 2 >= 0) cur_r -= 2; //If the position above has number
	}
	if((c == 'S' || c == 's') && cur_r < 3){	//Check if in a valid range
		if(editable[cur_r+1][cur_c]==1) cur_r += 1; //If the position below doesn't have number
		else if(editable[cur_r+1][cur_c]==0 && cur_r + 2 <= 3) cur_r += 2; //If the position below has number
	}
    return cur_r;
}

int move_cursorC(char c)
{
	if((c == 'A' || c == 'a') && cur_c > 0){	//Check if in a valid range
		if(editable[cur_r][cur_c-1]==1) cur_c -= 1; //If the position on the left doesn't have number
		else if (editable[cur_r][cur_c-1]==0 && cur_c - 2 >= 0) cur_c -= 2; //If the position on the left has number
	}
	if((c == 'D' || c == 'd') && cur_c < 3){	//Check if in a valid range
		if(editable[cur_r][cur_c+1]==1) cur_c += 1; //If the position on the right doesn't have number
		else if(editable[cur_r][cur_c+1]==0 && cur_c + 2 <= 3) cur_c += 2; //If the position on the right has number
	}
    return cur_c;
}

bool is_invalid(int i, int j)
{
	if (error[i][j] == 0) return true;
	return false;
	
}

bool is_done(int i, int j)
{
	if(finished[i][j]) return true;
	else return false;
    /* TODO: Check if board[i][j] is in a line that has finished. */
}

bool check_win()
{
    /* TODO: Check if the game is set. That is, every cell is finished. */
    return false;
}

bool is_moving_action(char c)
{
    return (c == 'W' || c == 'w' || c == 'S' || c == 's' ||
            c == 'A' || c == 'a' || c == 'D' || c == 'd');
}

bool is_filling_action(char c)
{
    return (c >= '0' && c <= '4');
}

string get_styled_text(string text, string style)
{
    string color = "", font = "";
    for (char c : style)
    {
        if (c == 'R')
            color = "31";
        if (c == 'G')
            color = "32";
        if (c == 'E')
            color = "41";
        if (c == 'C')
            color = "106";
        if (c == 'B')
            font = ";1";
    }
    return "\x1b[" + color + font + "m" + text + "\x1b[0m";
}

void print_board()
{
    // Flush the screen
    cout << "\x1b[2J\x1b[1;1H";

    // Print usage hint.
    cout << get_styled_text("W/A/S/D: ", "B") << "move cursor" << endl;
    cout << get_styled_text("    1-4: ", "B") << "fill in number" << endl;
    cout << get_styled_text("      0: ", "B") << "clear the cell" << endl;
    cout << get_styled_text("      Q: ", "B") << "quit" << endl;
    cout << endl;

    // Iterate through and print each cell.
    for (int i = 0; i < 4; ++i)
    {
        // Print horizontal divider.
        if (i && i % 2 == 0)
            cout << "---------------" << endl;

        // Print the first vertical divider in each line.
        cout << "|";
        for (int j = 0; j < 4; ++j)
        {
            // Set text style based on the state of the cell.
            string style = "";

            // Set style for the cell the cursor pointing to.
            if (cur_r == i && cur_c == j)
                style = "C";
            // Set style for the cell in an invalid line.
            else if (is_invalid(i, j))
                style = "E";
            // Set style for the cell in a finished line.
            else if (is_done(i, j))
                style = "G";

            // Set style for a the cell that is immutable.
            if (!editable[i][j])
                style += "B";

            // Print the cell out in styled text.
            // If the content is 0, print a dot, else print the number.
            if (board[i][j] == 0)
                cout << get_styled_text(" ¡P ", style);
            else
                cout << get_styled_text(" " + to_string(board[i][j]) + " ", style);

            // Print the vertical divider for each block.
            if ((j + 1) % 2 == 0)
                cout << "|";
        }
        cout << endl;
    }
}

void initialize()
{
    // Set up styled text for Windows.
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);

    // Mark editable cells
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            editable[i][j] = !board[i][j];

    // Print the initial board out.
    print_board();
}


int main()
{
    char c;
    bool action_ok;
    initialize();
    
    while (cin >> c)
    {
        action_ok = false;
        if (is_moving_action(c))
        {
            action_ok = true;
            move_cursorR(c);
            move_cursorC(c);
        }

        if (is_filling_action(c))
        {
            action_ok = true;
            fill_number(c);
        }

        if (c == 'Q' || c == 'q')
            break;

        print_board();

        if (check_win())
        {
            cout << "YOU WIN!" << endl;
            break;
        }

        if (!action_ok)
            cout << get_styled_text("!!! Invalid action !!!", "R");
    }

    return 0;
}
