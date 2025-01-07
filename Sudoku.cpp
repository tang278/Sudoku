#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <ctime>

using namespace std;

int board[4][4] = {
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
};

bool editable[4][4];//Check if the cell is editable

bool error[4][4]={{1,1,1,1},{1,1,1,1},{1,1,1,1},{1,1,1,1}};//Check if the line or block has conflict number

bool finished[4][4]={{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};//Used to check if every cell is finished

bool cw[4][4]={{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};//Used to check win

bool finished_check_r[4][4]={{1,1,1,1},{1,1,1,1},{1,1,1,1},{1,1,1,1}};//Check if the row is finished

bool finished_check_c[4][4]={{1,1,1,1},{1,1,1,1},{1,1,1,1},{1,1,1,1}};//Check if the column is finished

bool finished_check_b[4][4]={{1,1,1,1},{1,1,1,1},{1,1,1,1},{1,1,1,1}};//Check if the block is finished

bool conflict_r, conflict_c, conflict_block;//Check if the line or block has conflict number

bool finished_r, finished_c, finished_block;//Check if the line or block is finished

int cur_r = 0, cur_c = 0;//The current position

void check_horizontal(int i)
{ 	
	//Check if the row has conflict number
	conflict_r = 0;
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
            error[i][m] = 1; // Reset the statement of this row
        }
    }
	//Check if the row is finished
	finished_r = 0;
	for(int a = 0; a < 4; ++a){
        if(board[i][a] == 0){ // If there is 0 in the current position of the row
            finished_r = 0;    // Not finished
            break;
        }
		else if(board[i][a] != 0){// If there isn't 0 in the current position of the row
  			for(int b = a+1; b < 4; ++b ){
    			if(board[i][a] != board[i][b] && board[i][b] != 0){ //Check each cell has different number and isn't 0
    			finished_r=1;
				}
				else if((board[i][a] == board[i][b])){ //If two cells have conflict number
				finished_r=0;
				} 
			}
		}
	} 
	if(finished_r){
		for(int m=0; m<4; ++m){
			finished[i][m]=1;
			cw[i][m]=1;
			finished_check_r[i][m]=1;
		}
	}//Mark this row as finished
	if(!finished_r){
		for(int m=0; m<4; ++m){
			finished_check_r[i][m]=0;
			cw[i][m]=0;
			if(finished_check_r[i][m]==1 || finished_check_b[i][m]==1 || finished_check_c[i][m]==1) continue; 
			finished[i][m]=0;//Don't change the statement of the cell which is finished in block or column.
		}
	}//Reset the statement of this row
	
}

void check_vertical(int i)
{
	//Check if the column has conflict number
	conflict_c = 0;
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
            error[m][i] = 1; // Reset the statement of this column
        }
    }
    finished_c = 0;
	//Check if the column is finished
	for(int a = 0; a < 4; ++a){
         if(board[a][i] == 0){ // If there is any 0 in the column
            finished_c = 0;    // Not finished
            break;
        }
		else if(board[a][i] != 0){// If there is no any 0 in the column
  			for(int b = a+1; b < 4; ++b ){
    			if(board[a][i] != board[b][i] && board[b][i] != 0){ //Check each cell has different number and isn't 0
    			finished_c=1;
				}
				else if((board[a][i] == board[b][i])){ //If two cells have conflict number
				finished_c=0;
				} 
			}
		}
	} 
	if(finished_c){
		for(int m=0; m<4; ++m){
			finished[m][i]=1;
			cw[m][i]=1;
			finished_check_c[m][i]=1;//Mark the column as finished
		}
	}
	if(!finished_c){
		for(int m=0; m<4; ++m){
			finished_check_c[m][i]=0;
			cw[m][i]=0;
			if(finished_check_c[m][i]==1 || finished_check_r[m][i]==1 || finished_check_b[m][i]==1) continue; 
			//Don't change the statement of the cell which is finished in block or row.
			finished[m][i]=0;
		}	//Reset the statement of the column
	}
}
	
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
    if (conflict_block) {//If the block has conflict number
        for(int a = start_r; a <= end_r; ++a) {
            for(int b = start_c; b <= end_c; ++b) {
                error[a][b] = 0;
            }
        }
    }
   
    if (!conflict_block) {//If the block has no conflict number
        for(int a = start_r; a <= end_r; ++a) {
            for(int b = start_c; b <= end_c; ++b) {
                error[a][b] = 1; //Reset the state of this block
            }
        }
        check_horizontal(cur_r);
		check_vertical(cur_c); //check again the lines are in the correct state
    }
    finished_block = 1;
    //Check if the block is finished
    for(int a = start_r; a <= end_r; ++a) {
        for(int b = start_c; b <= end_c; ++b) {
        	if(board[a][b] == 0){
        		finished_block = 0;
        		break;
			}
            else if(board[a][b] != 0) {	// while current cell isn't 0
                if(b + 1 <= end_c && (board[a][b] == board[a][b+1] || board[a][b+1] == 0)) {	//check the right cell
                    finished_block = 0;
                    break;
                }
                if(a + 1 <= end_r && (board[a][b] == board[a+1][b] || board[a+1][b] == 0)) {	//check the cell below
                    finished_block = 0;
                    break;
                }
                if((a + 1 <= end_r && b + 1 <= end_c) && (board[a][b] == board[a+1][b+1] || board[a+1][b+1] == 0)) {	//check the right cell below
                    finished_block = 0;
                    break;
                }
            }
        }
    }
    if (finished_block) {//If the block is finished
        for(int a = start_r; a <= end_r; ++a) {
            for(int b = start_c; b <= end_c; ++b) {
                finished[a][b] = 1;
                cw[a][b] = 1;
                finished_check_b[a][b] = 1;
            }
        }
    }
   
    if (!finished_block) {//If the block isn't finished
        for(int a = start_r; a <= end_r; ++a) {
            for(int b = start_c; b <= end_c; ++b) {
            	finished_check_b[a][b] = 0;
            	cw[a][b] = 0;
            	if(finished_check_b[a][b]==1 || finished_check_r[a][b]==1 || finished_check_c[a][b]==1) continue;
            	//Don't change the statement of the cell which is finished in column or row.
                finished[a][b] = 0; //Reset the state of this block
            }
        }
    }

}

void fill_number(int c)
{
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
}

bool check_win()
{
	bool check = 0;
	for(int i = 0; i < 4; ++i){
		for(int j = 0; j < 4; ++j){
			if(cw[i][j] == 1) check = 1;
			if(cw[i][j] == 0) {
				check = 0;
				return check;
			}
		}
	}
	return check;
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

//Backtracking for answer
bool is_ok(int i, int j, int num) {
    //check column
    for (int m = 0; m < 4; ++m) {
        if (board[i][m] == num) {
            return false;
        }
    }

    //check row
    for (int n = 0; n < 4; ++n) {
        if (board[n][j] == num) {
            return false;
        }
    }
    
    
    int start_r;
    int start_c;
	switch(i) {
    case 0: start_r = 0; break;
    case 1: start_r = 0; break;
    case 2: start_r = 2; break;
    case 3: start_r = 2; break;
    }
    switch(j) {
    case 0: start_c = 0; break;
    case 1: start_c = 0; break;
    case 2: start_c = 2; break;
    case 3: start_c = 2; break;
    }
    for (int a = start_r; a < start_r + 2; ++a) {
        for (int b = start_c; b < start_c + 2; ++b) {
            if (board[a][b] == num) {
                return false;
            }
        }
    }

    return true;
}
bool solve_sudoku() {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            // If the cell is empty
            if (board[i][j] == 0) {
                for (int num = 1; num <= 4; ++num) {
                    if (is_ok(i, j, num)) {
                        board[i][j] = num;

                        // Backtracking
                        if (solve_sudoku()) {
                            return true;
                        }

                        // If it doesn't work, reset the cell and try the next number
                        board[i][j] = 0;
                    }
                }
                return false; // If no number is valid, return false
            }
        }
    }
    return true; // All cells are filled correctly
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
    cout << get_styled_text("      P: ", "B") << "answer" << endl;
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

void generate_sudoku(int num_empty_cells) {
    // Generate a complete solution first
    if (!solve_sudoku()) {
    	solve_sudoku();
        return;
    }

    // Now, remove 'num_empty_cells' numbers to create the puzzle
    srand(time(0)); // Seed for random number generation
    int removed = 0;
    while (removed < num_empty_cells) {
        int i = rand() % 4; // Random row
        int j = rand() % 4; // Random column

        if (board[i][j] != 0) { // If it's not already empty
            board[i][j] = 0; // Remove the number
            removed++;
        }
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

	// Generate a sudoku puzzle with a specific number of empty cells
    int num_empty_cells = 12; // Number of cells to leave empty in the puzzle
    generate_sudoku(num_empty_cells);
    
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
        
		if (c == 'p' || c == 'P'){
			initialize();
			bool wow = solve_sudoku();
			if(wow)	print_board();
			break;
		}
    }

    return 0;
}
