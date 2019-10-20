/***************
@ brief :  TEARS.C
@ auther:  KXT
@ date  :  2016.7.23
***************/

# include "tears.h"
# include "lcd.h"
# include "timer.h"
# include "delay.h"
# include "led.h"
# include "key.h"

extern u32 Interface_ChangeFLAG;

static u32 Block[19][32];
static u32 Bkp_Block[19][32];

u32 First_InformFlag = 0;    //新图形形成标志
const u32 shape_color[8] = {RED,BLUE,LGRAYBLUE,BROWN,GREEN,MAGENTA,GRED,CYAN};  //图形颜色和背景色
_Shape NewShape = {10,3,1,0};

//每个图形向左/右/下方向上的方块序号
//排序分别为 左 右 下
u32 sh_block[][4] = {
/*NUM 0:*/	        {0,2},{1,3},{2,3},
/*NUM 0:*/	        {0,2},{1,3},{2,3},
/*NUM 0:*/	        {0,2},{1,3},{2,3},
/*NUM 0:*/	        {0,2},{1,3},{2,3},

/*NUM 1--type 0:*/	{1},{3},{1,0,2,3},
/*NUM 1--type 1:*/	{1,0,2,3},{1,0,2,3},{3},
/*NUM 1--type 2:*/	{1},{3},{1,0,2,3},
/*NUM 1--type 3:*/	{1,0,2,3},{1,0,2,3},{3},

/*NUM 2--type 0:*/ 	{1,2},{1,3},{2,0,3},
/*NUM 2--type 1:*/	{2,0,3},{2,1,3},{3,1},
/*NUM 2--type 2:*/	{1,2},{1,3},{2,1,3},
/*NUM 2--type 3:*/	{2,1,3},{2,0,3},{1,3},

/*NUM 3--type 0:*/ 	{1,2},{0,3},{1,2,3},
/*NUM 3--type 1:*/	{1,2,3},{1,0,3},{3,0},
/*NUM 3--type 0:*/ 	{1,2},{0,3},{1,2,3},
/*NUM 3--type 1:*/	{1,2,3},{1,0,3},{3,0},

/*NUM 4--type 0:*/	{0,2},{1,3},{2,3,1},
/*NUM 4--type 1:*/	{2,1,3},{1,0,3},{2,1},
/*NUM 4--type 2:*/	{0,2},{1,3},{2,3,1},
/*NUM 4--type 3:*/	{2,1,3},{1,0,3},{2,1},

/*NUM 5--type 0:*/ 	{0,1},{1,3},{0,2,3},
/*NUM 5--type 1:*/	{2,0,3},{2,1,3},{3,1},
/*NUM 5--type 2:*/	{1,3},{1,0},{3,2,1},
/*NUM 5--type 3:*/	{2,1,3},{2,0,3},{1,0},

/*NUM 6--type 0:*/ 	{1,3},{1,0},{3,2,0},
/*NUM 6--type 1:*/	{2,0,3},{2,1,3},{0,1},
/*NUM 6--type 2:*/	{0,1},{1,3},{1,2,3},
/*NUM 6--type 3:*/	{2,1,3},{2,0,3},{1,3},
};

 u32 dir_lines[][3] = {
/*NUM 0:*/	        {2,2,2},
/*NUM 0:*/	        {2,2,2},
/*NUM 0:*/	        {2,2,2},
/*NUM 0:*/	        {2,2,2},


/*NUM 1--type 0:*/	{1,1,4},
/*NUM 1--type 1:*/	{4,4,1},
/*NUM 1--type 2:*/	{1,1,4},
/*NUM 1--type 3:*/	{4,4,1},

/*NUM 2--type 0:*/ 	{2,2,3},
/*NUM 2--type 1:*/	{3,3,2},
/*NUM 2--type 2:*/	{2,2,3},
/*NUM 2--type 3:*/	{3,3,2},

/*NUM 3--type 0:*/ 	{2,2,3},
/*NUM 3--type 1:*/	{3,3,2},
/*NUM 3--type 2:*/ 	{2,2,3},
/*NUM 3--type 3:*/	{3,3,2},

/*NUM 4--type 0:*/	{2,2,3},
/*NUM 4--type 1:*/	{3,3,2},
/*NUM 4--type 2:*/	{2,2,3},
/*NUM 4--type 3:*/	{3,3,2},

/*NUM 5--type 0:*/ 	{2,2,3},
/*NUM 5--type 1:*/	{3,3,2},
/*NUM 5--type 2:*/	{2,2,3},
/*NUM 5--type 3:*/	{3,3,2},

/*NUM 6--type 0:*/ 	{2,2,3},
/*NUM 6--type 1:*/	{3,3,2},
/*NUM 6--type 2:*/	{2,2,3},
/*NUM 6--type 3:*/	{3,3,2},
};

//User游戏数据的存储与显示
extern _Game *User_Date(void)
{
	static _Game date = {1,0}; 
	return &date;
}
//显示用户的分数和游戏等级信息
extern void Show_UserDate(_Game *date)
{
	POINT_COLOR = BLACK;
	LCD_ShowString(192,200,36,16,16,"score");
	LCD_ShowNum(192,220,date->score,6,12);
	
	LCD_ShowString(192,240,90,16,16,"level");
	LCD_ShowNum(192,260,date->level,6,12);
}

//更新游戏等级
extern void Update_UserLevel(_Game *date)
{
	if(date->score > Pass_Score)
	{
		date->score = 0;
		date->level+=1;
		if(date->level<=6)
			LCD_ShowString(70,100,90,16,16,"Next_Level");
		else if(date->level >6)
		{
			date->level = 0;
			LCD_ShowString(40,100,130,16,16,"The Game is Over!");
		}
		delay_ms(5000);
		Int_Block(); 
		Game_Interface();
	}
}

//游戏界面的轮廓生成
extern void Game_Interface(void)
{
	static u8 i = 0;
	LCD_Clear(CYAN);
	POINT_COLOR = BLUE;
	Show_UserDate(User_Date());  //显示用户游戏信息
	for(i=1;i<18;i++)
	{
		Draw_OneBlock(i,31,BRED);   //底部行17个
		Block[i][31] = BRED;
	}	
	for(i=0;i<32;i++)               //两侧列32个
	{
		Draw_OneBlock(0,i,BRED);
		Draw_OneBlock(18,i,BRED);
		Block[0][i] = BRED;  
		Block[18][i] = BRED; 
	}
	LCD_Fill(210,0,240,30,GRAY);
	LCD_Fill(210,290,240,320,GRAY);
}

//对全图的两个数组进行初始化
extern void Int_Block(void)
{
	u32 x=0;
	u32 y = 0;
	for(x=0;x<19;x++)
	{
		for(y=0;y<32;y++)
		{
			Block[x][y] = CYAN;
			Bkp_Block[x][y] = CYAN;
		}
	}
}

//选择新游戏或者是进行设定游戏速度之后再开始
void Select_StartSpeed(void)
{
	u8 key;
	static _Game *date;
	static u32 select_flag = 0;
	static u32 start_flag = 0;
	
	static u8 Open_Start_FLAG = 0;
	static u8 Open_SELECT_FLAG = 1;
	static u8 Open_B_FLAG = 0;
	
	date = User_Date();

	//WKUP设置换位
	//KEY0开始
	//KEY1选择
	key=KEY_Scan(0);
	if(key)//没有按下键不会进去
	{
		if(1 == Open_SELECT_FLAG)
		{
			if(3 == key)  //设置换位
			{
				select_flag++;
				select_flag%=2;
				Open_Start_FLAG = 1;
			}
		}
		if(1 == Open_Start_FLAG)
		{
			if(1 == key)  //开始
			{
				switch(select_flag)
				{
					case 0:start_flag = 1;break;
					case 1:start_flag = 2;break;
				}
			}
		}
		if(1 == Open_B_FLAG)
		{
			if(3 == key)  //速度
			{
				date->level+=1;
				if(date->level>=7)
					date->level = 0;
			}
		}	
	}
	
	if(0 == select_flag)
	{
		LCD_Fill(50,155,65,160,WHITE);   //消除上一个选择标志   
		LCD_Fill(50,122,65,127,BLACK);	//显示当前选择标志  选择开始
		if(1 == start_flag)
		{
			start_flag = 0;
			Interface_ChangeFLAG = 0;
		}
	}
	else if(1 == select_flag)
	{	
		if(2 != start_flag)                  
		{
			LCD_Fill(50,122,65,127,WHITE);   //消除上一个选择标志
			LCD_Fill(50,155,65,160,BLACK);   //显示当前选择      选择设置SPEED
		}
		if(2 == start_flag)
		{
			if(Open_SELECT_FLAG)              //加入此判断句为了只清屏一次
			{
				LCD_Fill(50,155,65,160,WHITE); 
				LCD_Clear(WHITE);            //清除一下屏幕
				Open_SELECT_FLAG = 0;        //不允许SELECT按键再起作用
			}	
			POINT_COLOR = BLACK;
			LCD_ShowString(20,110,200,16,16,"LEVEL: ");
			LCD_ShowNum(70,110,date->level,16,16);
			POINT_COLOR = RED;
			LCD_ShowString(15,170,200,16,16,"Then press KEY0 to start--");
			Open_B_FLAG = 1;                //开启B选择速度功能
			if(1 == KEY_Scan(0))  //确定
				Interface_ChangeFLAG = 0;
		}
	}
}

/*******装载第1第2个小块*********/
//每个图像的小块序号分布详细见 附件--图形方块排序.word
static void Load_Block1(Position * p_blcok,u32 x1,u32 y1,int x_move,int xi_times,int y_move,int yi_times)
{
	static u8 i = 0;
	for(i=0;i<2;i++)
	{
		p_blcok[i+0].x = x1+x_move+i*xi_times;
		p_blcok[i+0].y = y1+y_move+i*yi_times;
	}
}

/*******装载第3第4个小块*********/
static void Load_Block2(Position * p_blcok,u32 x1,u32 y1,int x_move,int xi_times,int y_move,int yi_times)
{
	static u8 i = 0;
	for(i=0;i<2;i++)
	{
		p_blcok[i+2].x = x1+x_move+i*xi_times;
		p_blcok[i+2].y = y1+y_move+i*yi_times;
	}
}


/**************************************/
//           Shape0-正方体
//返回存储Shape0各个方块位置信息的指针
extern Position* Shape0(u32 x1,u32 y1,u32 types)
{	                                                     
	static Position Cur_BLOCK[4];
	Load_Block1(Cur_BLOCK,x1,y1,0,1,0,0);   
	Load_Block2(Cur_BLOCK,x1,y1,0,1,1,0);
	return Cur_BLOCK;
}

//           Shape1-条形
extern Position* Shape1(u32 x1,u32 y1,u32 types)  
{	
	static Position Cur_BLOCK[4];
	if(0==types || 2==types)                
	{
		Load_Block1(Cur_BLOCK,x1,y1,0,-1,0,0);
		Load_Block2(Cur_BLOCK,x1,y1,1,1,0,0);
	}
	else if(1==types || 3==types)
	{                                   
		Load_Block1(Cur_BLOCK,x1,y1,0,0,0,-1);
		Load_Block2(Cur_BLOCK,x1,y1,0,0,1,1);
	}
	return Cur_BLOCK;
}

//           Shape2-土形
extern Position* Shape2(u32 x1,u32 y1,u32 types)
{
	static Position Cur_BLOCK[4];
	switch(types)
	{
		case 0:
				Load_Block1(Cur_BLOCK,x1,y1,0,0,0,-1);
				Load_Block2(Cur_BLOCK,x1,y1,-1,2,0,0);break;
		case 1:
				Load_Block1(Cur_BLOCK,x1,y1,0,1,0,0);
				Load_Block2(Cur_BLOCK,x1,y1,0,0,-1,2);break;
		case 2:
				Load_Block1(Cur_BLOCK,x1,y1,0,0,0,1);
				Load_Block2(Cur_BLOCK,x1,y1,-1,2,0,0);break;
		case 3:
				Load_Block1(Cur_BLOCK,x1,y1,0,-1,0,0);
				Load_Block2(Cur_BLOCK,x1,y1,0,0,-1,2);break;
	}
	return Cur_BLOCK;
}

//           Shape3-闪电形
extern Position* Shape3(u32 x1,u32 y1,u32 types)
{
	static Position Cur_BLOCK[4];
	if(0==types || 2==types)
	{		
		Load_Block1(Cur_BLOCK,x1,y1,0,-1,0,0);
		Load_Block2(Cur_BLOCK,x1,y1,0,1,1,0);
	}
	else if(1==types || 3==types)
	{													            
		Load_Block1(Cur_BLOCK,x1,y1,0,0,0,-1);
		Load_Block2(Cur_BLOCK,x1,y1,-1,0,0,1);               
	}
	return Cur_BLOCK;
}

//           Shape4-闪电形
extern Position* Shape4(u32 x1,u32 y1,u32 types)
{
	static Position Cur_BLOCK[4];
	if(0==types || 2==types)
	{	
		Load_Block1(Cur_BLOCK,x1,y1,0,1,0,0);
		Load_Block2(Cur_BLOCK,x1,y1,-1,1,1,0);   
	}
	else if(1==types || 3==types)
	{	                                                         
		Load_Block1(Cur_BLOCK,x1,y1,0,0,0,1);
		Load_Block2(Cur_BLOCK,x1,y1,-1,0,0,-1); 
	}
	return Cur_BLOCK;
}

//           Shape5-L形
extern Position* Shape5(u32 x1,u32 y1,u32 types)
{
	static Position Cur_BLOCK[4];
	switch (types)
	{
		case 0:                                                                        
				Load_Block1(Cur_BLOCK,x1,y1,0,0,0,-1);
				Load_Block2(Cur_BLOCK,x1,y1,1,1,0,0); break;
		case 1:                                                                                 
				Load_Block1(Cur_BLOCK,x1,y1,0,1,0,0);
				Load_Block2(Cur_BLOCK,x1,y1,0,0,1,1); break;
		case 2:                                        
				Load_Block1(Cur_BLOCK,x1,y1,0,0,0,1);
				Load_Block2(Cur_BLOCK,x1,y1,-1,-1,0,0);break;
		case 3:                                                
				Load_Block1(Cur_BLOCK,x1,y1,0,-1,0,0);
				Load_Block2(Cur_BLOCK,x1,y1,0,0,-1,-1);break;
	}
	return Cur_BLOCK;
}

//           Shape6-L形
extern Position* Shape6(u32 x1,u32 y1,u32 types)
{
	static Position Cur_BLOCK[4];
	switch (types)
	{                                                          
		case 0:                                                                        
				Load_Block1(Cur_BLOCK,x1,y1,0,0,0,-1);
				Load_Block2(Cur_BLOCK,x1,y1,-1,-1,0,0); break;
		case 1:                                                                                 
				Load_Block1(Cur_BLOCK,x1,y1,0,1,0,0);
				Load_Block2(Cur_BLOCK,x1,y1,0,0,-1,-1); break;
		case 2:                                        
				Load_Block1(Cur_BLOCK,x1,y1,0,0,0,1);
				Load_Block2(Cur_BLOCK,x1,y1,1,1,0,0);break;
		case 3:                                                
				Load_Block1(Cur_BLOCK,x1,y1,0,-1,0,0);
				Load_Block2(Cur_BLOCK,x1,y1,0,0,1,1);break;
	}
	return Cur_BLOCK;
}

//画出图形函数
static void Draw_OneBlock(u32 x,u32 y,u32 color)
{
	static u16 real_x = 0;
	static u16 real_y = 0;
	real_x = x*10;real_y = y*10;
	POINT_COLOR = color;
	LCD_DrawRectangle(real_x,real_y,real_x+9,real_y+9);      //画外围方框
	LCD_Fill(real_x+2,real_y+2,real_x+9-2,real_y+9-2,color); //填充方框
}

static void Draw_OneShape(Position *p_block,u32 color)
{
	static u8 i = 0;
	for(i=0;i<4;i++)
	{
		Draw_OneBlock(p_block->x,p_block->y,color);
		p_block++;
	}
}

// 装载当前图形的位置信息
// @draw_Or_Erase ：显示或是擦出图形
// 返回当前装载的图形的位置指针
extern Position* LOad_Cur_Block(_Shape *New)
{
	static Position *Cur_Block;
	switch(New->Num)
	{
		case 0:Cur_Block = Shape0(New->x,New->y,New->Types);break;
		case 1:Cur_Block = Shape1(New->x,New->y,New->Types);break;
		case 2:Cur_Block = Shape2(New->x,New->y,New->Types);break;
		case 3:Cur_Block = Shape3(New->x,New->y,New->Types);break;
		case 4:Cur_Block = Shape4(New->x,New->y,New->Types);break;
		case 5:Cur_Block = Shape5(New->x,New->y,New->Types);break;
		case 6:Cur_Block = Shape6(New->x,New->y,New->Types);break;
	}
	return Cur_Block;        
}

//图形的生成或者擦除
extern void Shape_Inform(_Shape *New,u32 Draw_OR_Erase)
{
	static Position* p_block;

	if(Draw == Draw_OR_Erase)
	{
			p_block = Position_Correct(New);
		switch(New->Num)
		{
			case 0:New->color = shape_color[0];break;
			case 1:New->color = shape_color[1];break;
			case 2:New->color = shape_color[2];break;
			case 3:New->color = shape_color[3];break;
			case 4:New->color = shape_color[4];break;
			case 5:New->color = shape_color[5];break;
			case 6:New->color = shape_color[6];break;
		}
	}
	else if(Erase == Draw_OR_Erase)
	{
		p_block = LOad_Cur_Block(New);
		switch(New->Num)
		{
			case 0:New->color = shape_color[7];break;
			case 1:New->color = shape_color[7];break;
			case 2:New->color = shape_color[7];break;
			case 3:New->color = shape_color[7];break;
			case 4:New->color = shape_color[7];break;
			case 5:New->color = shape_color[7];break;
			case 6:New->color = shape_color[7];break;
		}
	}
	Draw_OneShape(p_block,New->color);
}

//初始化移动标志位
extern _Move_Flag *Is_Flag(void)
{
	static _Move_Flag move = {1,1,1}; 
	return &move;
}

//读某位置的颜色
extern u32 Read(Position next_block)
{
	static u32 next_poin_x = 0;
	static u32 next_poin_y = 0;
	next_poin_x = next_block.x*10;
	next_poin_y = next_block.y*10;
	return LCD_ReadPoint(next_poin_x,next_poin_y);
}

//装载图形边界的小块坐标
extern Position* Load_NextBlock(Position *p,u32 *s,u32 lines,u32 dir)
{
	static u32 i = 0;
	static Position cache[4];
	if(left == dir)
	{
		for(i=0;i<lines;i++)
		{
			(cache+i)->x = (p+s[i])->x-1;  (cache+i)->y = (p+s[i])->y;
		}
	}
	else if(right == dir)
	{
		for(i=0;i<lines;i++)
		{
			(cache+i)->x = (p+s[i])->x+1; (cache+i)->y = (p+s[i])->y;	
		}
	}
	else if(down == dir)
	{
		for(i=0;i<lines;i++)
		{
			(cache+i)->x = (p+s[i])->x;   (cache+i)->y = (p+s[i])->y+1;
		}
	}
	return cache;
}

//设置移动标志位
extern void SetMove_Flag(Position *next_block,u32 lines,u32 dir)
{
	static u32 bkp[4],i;
	static _Move_Flag *move;
	static u32 sum = 0;
	move = Is_Flag();
	for(i=0;i<lines;i++)
	{
		if(CYAN == Read(*(next_block+i)))
			bkp[i] = 1;
		else if(CYAN != Read(*(next_block+i)))
			bkp[i] = 0;
		sum += bkp[i];
	}	
	switch(dir)
	{
		case left:  move->Left_Flag  = sum < lines ? 0:1;break;
		case right: move->Right_Flag = sum < lines ? 0:1;break;
		case down:  move->Down_Flag  = sum < lines ? 0:1;break;
	}
	sum = 0;
}

//读边界坐标并设定移动标志
extern void Load_NextAndSetflag(_Shape *New, u32 (*s)[4],u32 (*lines)[3])
{
	static u8 i =0;
	static Position *left_cache;
	static Position *right_cache;
	static Position *down_cache;
	static Position *Cur_Block;
	
	Cur_Block = Position_Correct(New);

	left_cache =  Load_NextBlock(Cur_Block,s[12*New->Num+3*New->Types+0],lines[New->Num*4+New->Types][0],left);  
	SetMove_Flag(left_cache,lines[New->Num*4+New->Types][0],left);
	for(i=0;i<4;i++)
	{
		(left_cache+i)->x = 0;
		(left_cache+i)->y = 0;
	}	
	
	right_cache = Load_NextBlock(Cur_Block,s[12*New->Num+3*New->Types+1],lines[New->Num*4+New->Types][1],right); 
	SetMove_Flag(right_cache,lines[New->Num*4+New->Types][1],right);
	for(i=0;i<4;i++)
	{
		(right_cache+i)->x = 0;
		(right_cache+i)->y = 0;
	}
	
	down_cache =  Load_NextBlock(Cur_Block,s[12*New->Num+3*New->Types+2],lines[New->Num*4+New->Types][2],down);  
	SetMove_Flag(down_cache, lines[New->Num*4+New->Types][2],down);
	for(i=0;i<4;i++)
	{
		(down_cache+i)->x = 0;
		(down_cache+i)->y = 0;
	}
}

//排序求最小值--图形消除之后的整体下移会用到
extern u16 Array_size(Position *p,u8 clear_lines)
{
	static u8 i = 0;
	static u16 min_y = 0;
	min_y = p->y;
	for(i=1;i<clear_lines;i++)
	{
		if(min_y  > (p+i)->y)
			min_y = (p+i)->y;
	}
	return min_y;
}

//一排堆满则进行消除图形
extern void Clear_Shape(_Shape *New)
{
	static Position* Cur_Block;
	static u8 x,y,clear_x,block_i;
	static u8 clear_flag,clear_lines;
	static Position line_y[4];
	static u16 min_y;
	
	static Position seat;
	static _Game *date;
	date = User_Date();
	Cur_Block = LOad_Cur_Block(New);
	for(block_i=0;block_i<4;block_i++)
	{
		Block[Cur_Block[block_i].x][Cur_Block[block_i].y] = shape_color[New->Num];
	}
	
	for(block_i=0;block_i<4;block_i++)
	{
		if(Read(Cur_Block[block_i]) == CYAN)
		{
			block_i++;
		}
		else
		{
			for(x=1;x<18;x++)
			{	
				seat.x = x;
				seat.y= Cur_Block[block_i].y;
				if(Read(seat) != CYAN)
				{
					clear_flag++;
				}
			}
			if(17 == clear_flag)
			{
				line_y[clear_lines++].y = Cur_Block[block_i].y;
				for(clear_x=1;clear_x<18;clear_x++)
				{
					Draw_OneBlock(clear_x,Cur_Block[block_i].y,CYAN);
					Block[clear_x][Cur_Block[block_i].y] = CYAN;
				}	
				date->score+=100;
				clear_flag = 0;
			}	
			else 
				clear_flag = 0;
		}
	}
	min_y = Array_size(line_y,clear_lines);
	if(0 != min_y)
	{
		for(y=0;y<min_y;y++)
		{
			for(x=1;x<18;x++)		
				Bkp_Block[x][y+clear_lines] = Block[x][y];   //将当前情况暂存到bkp中
		}	
		for(y=0;y<min_y+clear_lines;y++)
		{
			for(x=1;x<18;x++)                             //将图形下移并且刷新一下BLOCK；
			{
				Draw_OneBlock(x,y,Bkp_Block[x][y]);
				Block[x][y] = Bkp_Block[x][y];            //再将刷新后的值反馈给BLock
				Bkp_Block[x][y] = CYAN;
			}
		}
		min_y = 0;
	}
	clear_lines = 0;
	for(x = 0;x<4;x++)
	{
		line_y[x].y = 0;
	}
}

extern void Dis_NextShape(u32 num,u32 types,u32 Draw_OR_Erase)
{
	static _Shape Next_Shape = {21,5};
	static Position *Next_S;
	Next_Shape.Num = num;
	Next_Shape.Types = types;
	if(Draw == Draw_OR_Erase)
		Next_Shape.color = shape_color[num];
	else if(Erase == Draw_OR_Erase)
		Next_Shape.color = shape_color[7];
	Next_S = LOad_Cur_Block(&Next_Shape);
	Draw_OneShape(Next_S,Next_Shape.color);
}

//图形移动部分
extern void Shape_Down_Move(_Shape *New)
{
	static u32 next_shape_num = 0;
	static u32 next_shape_types = 0;
	static _Move_Flag * Move;   //用于指向移动标志函数进行调用
	
	Move = Is_Flag();	
	if(1 == Move->Down_Flag)
	{
		Shape_Inform(New,Erase);
		New->y+=1;
	}
	else if(0 == Move->Down_Flag)
	{	

		Clear_Shape(New);
		New->x = 9;
		New->y = 3;
		New->Num =  next_shape_num;
		New->Types =  next_shape_types;
		First_InformFlag = 1;
		
		Dis_NextShape(next_shape_num,next_shape_types,Erase);
		next_shape_num = (rand()+Systick_Count)%7;
		next_shape_types = (rand()+Systick_Count)%4;
		Dis_NextShape(next_shape_num,next_shape_types,Draw);
	}
}


extern u32 Set_level(_Game *date)
{
	static u32 time_interval = 0;
	switch(date->level)
	{
		case 0:time_interval = 560;break;
		case 1:time_interval = 480;break;
		case 2:time_interval = 400;break;
		case 3:time_interval = 320;break;
		case 4:time_interval = 240;break;
		case 5:time_interval = 160;break;
		case 6:time_interval = 80; break;
	}
	return time_interval;
}

extern void Gmae_Over(u32 Over_FLag)
{
	while(Over_FLag)
	{
		POINT_COLOR = BLACK;
		LCD_ShowString(50,30,100,8,16,"Game Over!!!!!");
		LCD_ShowString(20,50,150,8,16,"Please Press SELECT--");
		LCD_ShowString(60,70,100,8,16,"to Restart!!!");
		
		while(3 == KEY_Scan(0))
		{
			NVIC_SystemReset();       //系统重新开始
		}
	}
}

//图形位置矫正函数-即不允许覆盖已有的图形
extern Position *Position_Correct(_Shape *New)
{
	static u32 i = 0;
	static Position seat;
	static Position *Cur_Block;
	static u32 flag = 0;
	Cur_Block = LOad_Cur_Block(New);
	if(1 == First_InformFlag)
	{
		First_InformFlag = 0;
		for(i=0;i<4;i++)
		{
			seat.x = Cur_Block[i].x; seat.y = Cur_Block[i].y;
			if(CYAN == Read(seat))
			{
				flag ++;
			}
		}
		if(flag != 4)
		{
			Gmae_Over(1);
		}
		flag = 0;
	}
	else
	{
		for(i=0;i<4;i++)
		{
			if(Block[Cur_Block[i].x][Cur_Block[i].y] == CYAN)
			{
				flag++;
			}	
		}
		if(4 == flag)
		{
		}
		else
		{
			if(New->Types != 0)
			{
				New->Types-=1;
			}
			else 
			{
				New->Types = 3;;
			}
		}
		Cur_Block = LOad_Cur_Block(New);
		flag = 0;
	}
	return Cur_Block;
}

/*****各个按键的功能函数******/
void LEFT_Key_FunCation(void)    //左移按键的功能
{
	static _Move_Flag *s;
	s = Is_Flag();
	if(1 == s->Left_Flag)
	{
		Shape_Inform(&NewShape,Erase);
		NewShape.x-=1;
	LED0 = 0;
	}
	else if(0 == s->Left_Flag)
	{
		;
	}
}

void RIGHT_Key_FunCation(void)   //右移按键的功能
{
	static _Move_Flag *s;
	s = Is_Flag();
	if(1 == s->Right_Flag)
	{
		Shape_Inform(&NewShape,Erase);
		NewShape.x+=1;
		LED1 = 0;
	}
	else if(0 == s->Right_Flag)
	{
		;
	}
}

void A_Key_FunCation(void)  
{
	Shape_Inform(&NewShape,Erase);
	NewShape.Num+=1;
	NewShape.Num%=7;
}

void B_Key_FunCation(void)  
{
	Shape_Inform(&NewShape,Erase);
	NewShape.Types+=1;
	NewShape.Types%=4;
}




