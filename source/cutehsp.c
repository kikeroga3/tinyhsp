#define __HSPCUI__
//#define __HSPSTD__
//#define __HSPEXT__

/*
上記のどれか１つを定義(コメントイン)する

# 定義の説明
__HSPCUI__ : コンソール版
__HSPSTD__ :   ミニマム版
__HSPEXT__ : エクストラ版

# コンパイル方法

Windows(MinGW)の場合:
- コンソール版: gcc -static cutehsp.c -o cutehspcl
-   ミニマム版: gcc -static cutehsp.c -o cutehsp  -lopengl32 -lglfw3dll            -mwindows
- エクストラ版: gcc -static cutehsp.c -o cutehspx -lopengl32 -lglfw3dll -lopenal32 -mwindows

macOSの場合:
- コンソール版: clang cutehsp.c -o cutehspcl
-   ミニマム版: clang cutehsp.c -o cutehsp -lglfw -framework OpenGL
- エクストラ版: clang cutehsp.c -o cutehspx -lglfw -framework OpenGL -framework OpenAL

Linuxの場合:
- コンソール版: gcc cutehsp.c -o cutehspcl -lm
-   ミニマム版: gcc cutehsp.c -o cutehsp -lglfw3 -lX11 -lXrandr -lXinerama -lXi -lXxf86vm -lXcursor -lGL -lpthread -ldl -lm
- エクストラ版: gcc cutehsp.c -o cutehspx -lopenal -lglfw3 -lX11 -lXrandr -lXinerama -lXi -lXxf86vm -lXcursor -lGL -lpthread -ldl -lm

*/
//=============================================================

#ifdef _WIN32
#define __WINDOWS__
#endif
#ifdef _WIN64
#define __WINDOWS__
#endif

#ifdef __APPLE__
#define __MACOS__
#endif

#ifdef __linux
#define __LINUX__
#endif

#ifndef __HSPCUI__
#define __HSPGUI__
#endif

#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define _USE_MATH_DEFINES
#include <math.h>

#ifdef __HSPGUI__
#ifdef __MACOS__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif
#include <GLFW/glfw3.h>
#endif

#ifdef __HSPEXT__
#ifdef __WINDOWS__
#include <al.h>
#include <alc.h>
#endif
#ifdef __MACOS__
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#endif
#ifdef __LINUX__
#include <AL/al.h>
#include <AL/alc.h>
#endif
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif

#define NHSP_CONFIG_MEMLEAK_DETECTION (0) // メモリリーク発見用ユーティリティを有効化

// グローバルな変数
char* filename;
bool run_flag;
#ifdef __HSPGUI__
int current_pos_x;
int current_pos_y;
int current_color_r;
int current_color_g;
int current_color_b;
int current_mouse_x;
int current_mouse_y;
int current_mouse_down_left;
int current_mouse_down_right;
int screen_width;
int screen_height;
char* window_title;
unsigned char pixel_data[640 * 480 * 3 * 2];
bool redraw_flag;
bool close_flag;

// GLFW
GLFWwindow* window;
#ifdef __HSPEXT__
// Extra
stbtt_fontinfo font; //フォント情報
unsigned char* font_ttf_buffer[2000000]; // フォント用バッファ
int font_ascent = 0;
float font_scale = 0.0;
int font_baseline = 0;
int font_size = 24; // フォントサイズ
int font_threshold = 128; //しきい値
double font_half_space_ratio = 0.4; //半角スペースの比率
double font_full_space_ratio = 0.8; //全角スペースの比率
bool font_smooth = true; //フォントのアンチエイリアシング
// OpenAL
ALCdevice* al_device; // デバイス
ALCcontext* al_context; // コンテキスト
#endif
#endif

// 全体
void initialize_system();
void destroy_system();

// メモリ
void* xmalloc(size_t size, const char* block_name/* = nullptr*/); //※ 引数の初期値
void xfree(void* ptr);
void* xrealloc(void* ptr, size_t size);

// リスト
typedef struct list_node_tag list_node_t;
struct list_node_tag
{
	list_node_t* prev_;
	list_node_t* next_;
	void* value_;
};

list_node_t* create_list_node();
void destroy_list_node(list_node_t* node);
void link_next(list_node_t* node, list_node_t* list);
void link_prev(list_node_t* node, list_node_t* list);
bool unlink_list_node(list_node_t* node);

typedef struct list_tag list_t;
struct list_tag
{
	list_node_t* head_;
	list_node_t* tail_;
};

list_t* create_list();
void destroy_list(list_t* list);
void list_prepend(list_t* list, list_node_t* node);
void list_append(list_t* list, list_node_t* node);
void list_erase(list_t* list, list_node_t* node);
list_node_t* list_find(list_t* list, void* value);
void list_free_all(list_t* list);

// キーワード
typedef enum
{
	KEYWORD_END = 0,
	KEYWORD_RETURN,
	KEYWORD_GOTO,
	KEYWORD_GOSUB,
	KEYWORD_REPEAT,
	KEYWORD_LOOP,
	KEYWORD_CONTINUE,
	KEYWORD_BREAK,
	KEYWORD_IF,
	KEYWORD_ELSE,
	MAX_KEYWORD,
} keyword_tag;

int query_keyword(const char* s);

// トークナイザ
typedef enum
{
	TOKEN_UNKNOWN = -1,
	TOKEN_EOF = 0,
	TOKEN_EOL,
	TOKEN_EOS,
	TOKEN_LBRACE,
	TOKEN_RBRACE,
	TOKEN_LPARENTHESIS,
	TOKEN_RPARENTHESIS,
	TOKEN_COMMA,
	TOKEN_INTEGER,
	TOKEN_REAL,
	TOKEN_STRING,
	TOKEN_OP_BOR,
	TOKEN_OP_BAND,
	TOKEN_OP_EQ,
	TOKEN_OP_NEQ,
	TOKEN_OP_GT,
	TOKEN_OP_GTOE,
	TOKEN_OP_LT,
	TOKEN_OP_LTOE,
	TOKEN_OP_ADD,
	TOKEN_OP_SUB,
	TOKEN_OP_MUL,
	TOKEN_OP_DIV,
	TOKEN_OP_MOD,
	TOKEN_ASSIGN,
	TOKEN_IDENTIFIER,
	MAX_TOKEN,
} token_tag;

typedef struct token_tag_ token_t;
struct token_tag_
{
	token_tag tag_;
	char* content_;
	int cursor_begin_, cursor_end_;
	int appear_line_;
	bool left_space_, right_space_;
};

typedef struct tokenize_context_tag tokenize_context_t;
struct tokenize_context_tag
{
	const char* script_;
	int cursor_;
	int line_;
	const char* line_head_;
};

int query_token_shadow(const char* ident, size_t len);
void initialize_tokenize_context(tokenize_context_t* c, const char* script);
void uninitialize_tokenize_context(tokenize_context_t* c);
token_t* get_token(tokenize_context_t* c);
void destroy_token(token_t* t);
char* create_token_string(const char* str, size_t len);

// パーサ
typedef enum
{
	NODE_EMPTY = 0,
	NODE_LABEL,
	NODE_BLOCK_STATEMENTS,
	NODE_COMMAND,
	NODE_ARGUMENTS,
	NODE_ASSIGN,
	NODE_VARIABLE,
	NODE_EXPRESSION,
	NODE_BOR,
	NODE_BAND,
	NODE_EQ,
	NODE_NEQ,
	NODE_GT,
	NODE_GTOE,
	NODE_LT,
	NODE_LTOE,
	NODE_ADD,
	NODE_SUB,
	NODE_MUL,
	NODE_DIV,
	NODE_MOD,
	NODE_UNARY_MINUS,
	NODE_PRIMITIVE_VALUE,
	NODE_IDENTIFIER_EXPR,
	NODE_END,
	NODE_RETURN,
	NODE_GOTO,
	NODE_GOSUB,
	NODE_REPEAT,
	NODE_REPEAT_CHECK,
	NODE_LOOP,
	NODE_CONTINUE,
	NODE_BREAK,
	NODE_IF,
	NODE_IF_DISPATCHER,
	NODE_IF_CHECK,
	NODE_JUMP_LABEL,
	NODE_JUMP_INTERNAL,
	MAX_NODE,
} node_tag;

typedef enum
{
	NODE_FLAG_ADDITIONAL = 1,
} ast_node_flag_tag;

typedef struct ast_node_tag ast_node_t;
struct ast_node_tag
{
	node_tag tag_;
	token_t* token_;
	ast_node_t *left_, *right_;
	unsigned int flag_;
	void* ext_;
};

typedef struct parse_context_tag parse_context_t;
struct parse_context_tag
{
	list_t* token_list_;
	list_node_t* token_current_;
	tokenize_context_t* tokenize_context_;
};

parse_context_t* create_parse_context();
void destroy_parse_context(parse_context_t* p);
void initialize_parse_context(parse_context_t* c, tokenize_context_t* t);
void uninitialize_parse_context(parse_context_t* c);
token_t* read_token(parse_context_t* c);
void unread_token(parse_context_t* c, size_t num);
token_t* prev_token(parse_context_t* c, size_t num);
ast_node_t* create_ast_node(node_tag tag, ast_node_t* left, ast_node_t* right);
ast_node_t* create_ast_node2(node_tag tag, token_t* token, ast_node_t* left);
void destroy_ast_node(ast_node_t* node);
bool is_eos_like_token(token_tag tag);
list_t* parse_script(parse_context_t* c);
void destroy_ast(list_t* ast);
ast_node_t* parse_statement(parse_context_t* c);
ast_node_t* parse_label_safe(parse_context_t* c);
ast_node_t* parse_control_safe(parse_context_t* c);
ast_node_t* parse_command_safe(parse_context_t* c);
ast_node_t* parse_arguments(parse_context_t* c);
ast_node_t* parse_assign_safe(parse_context_t* c);
ast_node_t* parse_variable_safe(parse_context_t* c);
ast_node_t* parse_expression(parse_context_t* c);
ast_node_t* parse_borand(parse_context_t* c);
ast_node_t* parse_eqneq(parse_context_t* c);
ast_node_t* parse_gtlt(parse_context_t* c);
ast_node_t* parse_addsub(parse_context_t* c);
ast_node_t* parse_muldivmod(parse_context_t* c);
ast_node_t* parse_term(parse_context_t* c);
ast_node_t* parse_primitive(parse_context_t* c);
ast_node_t* parse_identifier_expression(parse_context_t* c);

// 変数
typedef enum
{
	VALUE_NONE,
	VALUE_INT,
	VALUE_DOUBLE,
	VALUE_STRING,
	VALUE_VARIABLE,
} value_tag;

typedef struct value_tag_ value_t_;
struct value_tag_;

typedef struct variable_tag variable_t;
struct variable_tag
{
	char* name_;
	value_tag type_;
	int granule_size_;
	int length_;
	void* data_;
};

variable_t* create_variable(const char* name);
void destroy_variable(variable_t* v);
void prepare_variable(variable_t* v, value_tag type, int granule_size, int length);
list_t* create_variable_table();
void destroy_variable_table(list_t* table);
variable_t* search_variable(list_t* table, const char* name);
//移動

// 値（即値）
typedef struct value_tag__ value_t;
struct value_tag__
{
	value_tag type_;
	union
	{
		int ivalue_;
		double dvalue_;
		char* svalue_;
		struct
		{
			variable_t* variable_;
			int index_;
		};
		unsigned long long value_;
	};
};

//移動
void variable_set(list_t* table, const value_t* v, const char* name, int idx);
void* variable_data_ptr(const variable_t* v, int idx);
int variable_calc_int(const variable_t* r, int idx);
double variable_calc_double(const variable_t* r, int idx);
char* variable_calc_string(const variable_t* r, int idx);

value_t* create_value(int v); //※ 同名の関数
value_t* create_value2(double v);
value_t* create_value3(const char* v);
value_t* create_value4(variable_t* v, int idx);
value_t* create_value5(const value_t* v);
value_t* create_value_move(char* v);
void destroy_value(value_t* t);
void value_set(value_t* v, int i); //※ 同名の関数
void value_set2(value_t* v, double d);
void value_set3(value_t* v, const char* s);
void value_move(value_t* v, char* s); //※ 同名の関数
void value_move2(value_t* to, value_t* from);
value_tag value_get_primitive_tag(const value_t* r);
bool value_calc_boolean(const value_t* r);
int value_calc_int(const value_t* r);
double value_calc_double(const value_t* r);
char* value_calc_string(const value_t* r);
value_t* value_convert_type(value_tag to, const value_t* r);
void value_isolate(value_t* v);
void value_bor(value_t* v, const value_t* r);
void value_band(value_t* v, const value_t* r);
void value_eq(value_t* v, const value_t* r);
void value_neq(value_t* v, const value_t* r);
void value_gt(value_t* v, const value_t* r);
void value_gtoe(value_t* v, const value_t* r);
void value_lt(value_t* v, const value_t* r);
void value_ltoe(value_t* v, const value_t* r);
void value_add(value_t* v, const value_t* r);
void value_sub(value_t* v, const value_t* r);
void value_mul(value_t* v, const value_t* r);
void value_div(value_t* v, const value_t* r);
void value_mod(value_t* v, const value_t* r);

// スタック
typedef struct value_stack_tag value_stack_t;
struct value_stack_tag
{
	value_t** stack_;
	int top_;
	int max_;
};

value_stack_t* create_value_stack();
void destroy_value_stack(value_stack_t* st);
void initialize_value_stack(value_stack_t* st);
void uninitialize_value_stack(value_stack_t* st);
void stack_push(value_stack_t* st, value_t* v); //※ 同名の関数
void stack_push2(value_stack_t* st, const value_t* v);
value_t* stack_peek(value_stack_t* st, int i);
void stack_pop(value_stack_t* st, size_t n);

// システム変数
typedef enum
{
	SYSVAR_CNT,
	SYSVAR_STAT,
	SYSVAR_REFDVAL,
	SYSVAR_REFSTR,
	SYSVAR_STRSIZE,
#ifdef __HSPGUI__
	SYSVAR_MOUSEX,
	SYSVAR_MOUSEY,
	SYSVAR_MOUSEL,
	SYSVAR_MOUSER,
#endif
	MAX_SYSVAR,
} sysvar_tag;

int query_sysvar(const char* s);

// 実行環境
typedef struct label_node_tag label_node_t;
struct label_node_tag
{
	char* name_;
	list_node_t* statement_;
};

typedef struct call_frame_tag call_frame_t;
struct call_frame_tag
{
	list_node_t* caller_;
};

#define MAX_CALL_FRAME (16)

typedef struct loop_frame_tag loop_frame_t;
struct loop_frame_tag
{
	list_node_t* start_;
	int counter_;
	int max_;
	int cnt_;
};

#define MAX_LOOP_FRAME (16)

typedef struct execute_environment_tag execute_environment_t;
struct execute_environment_tag
{
	list_t* parser_list_;
	list_t* ast_list_;
	list_t* statement_list_;
	list_t* label_table_;
	list_t* variable_table_;
};

typedef struct execute_status_tag execute_status_t;
struct execute_status_tag
{
	value_stack_t* stack_;
	list_node_t* node_cur_;
	call_frame_t call_frame_[MAX_CALL_FRAME];
	int current_call_frame_;
	loop_frame_t loop_frame_[MAX_LOOP_FRAME];
	int current_loop_frame_;
	bool is_end_;
	int stat_;
	double refdval_;
	char* refstr_;
	int strsize_;
};

execute_environment_t* create_execute_environment();
void destroy_execute_environment(execute_environment_t* e);
void initialize_execute_status(execute_status_t* s);
void uninitialize_execute_status(execute_status_t* s);
void load_script(execute_environment_t* e, const char* script);
void evaluate(execute_environment_t* e, execute_status_t* s, ast_node_t* n);
void execute(execute_environment_t* e);

// ビルトイン
typedef void(*command_delegate)(execute_environment_t* e, execute_status_t* s, int arg_num);
typedef enum
{
	COMMAND_DIM = 0,
	COMMAND_DDIM,
	COMMAND_SDIM,
	COMMAND_BLOAD,
	COMMAND_BSAVE,
	COMMAND_POKE,
	COMMAND_RUN,
#ifdef __HSPCUI__
	COMMAND_INPUT,
#endif
#ifndef __HSPSTD__
	COMMAND_MES,
#endif
#ifdef __HSPGUI__
	COMMAND_WAIT,
	COMMAND_STOP,
	COMMAND_TITLE,
	COMMAND_PSET,
	COMMAND_REDRAW,
	COMMAND_POS,
	COMMAND_COLOR,
	COMMAND_LINE,
	COMMAND_BOXF,
	COMMAND_STICK,
	COMMAND_CIRCLE,
#ifdef __HSPEXT__
	COMMAND_FONT,
	COMMAND_PICLOAD,
	COMMAND_BEEP,
#endif
#endif
	MAX_COMMAND,
} builtin_command_tag;

int query_command(const char* s);
command_delegate get_command_delegate(builtin_command_tag command);
typedef void(*function_delegate)(execute_environment_t* e, execute_status_t* s, int arg_num);
typedef enum
{
	FUNCTION_INT = 0,
	FUNCTION_DOUBLE,
	FUNCTION_STR,
	FUNCTION_RND,
	FUNCTION_ABS,
	FUNCTION_POWF,
	FUNCTION_PEEK,
	MAX_FUNCTION,
} builtin_function_tag;

int query_function(const char* s);
function_delegate get_function_delegate(builtin_function_tag command);

// エラー
void
raise_error(const char* message, ...)
{
	char c[512];
	for (int i = 0; i < 512; i++) {
		c[i] = '\0';
	}
	va_list args;
	va_start(args, message);
	vsprintf(c, message, args);

#ifdef __HSPGUI__
	// タイトルバーにエラー表示する
	glfwSetWindowTitle(window, c);
	// 画面クローズまで待つ
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
	}
#else
	printf("Error Occurred!\n");
	vfprintf(stderr, message, args);
	printf("\nPress Enter key to continue...");
	fgetc(stdin);
#endif

	va_end(args);
	exit(-1);
}

//========================================================
// 描画系ユーティリティ

#ifdef __HSPGUI__
typedef struct {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
	uint8_t alpha;
} color_t;

color_t get_pixel_color(uint8_t *pixel_data,
	int32_t point_x, int32_t point_y,
	int32_t canvas_size_width, int32_t canvas_size_height)
{
	int index = (canvas_size_height - point_y) * canvas_size_width * 3 + point_x * 3;
	color_t color;
	color.red = pixel_data[index];
	color.green = pixel_data[index + 1];
	color.blue = pixel_data[index + 2];
	color.alpha = 255;
	return color;
}

void set_pixel_rgb(uint8_t *pixel_data,
	int32_t point_x, int32_t point_y,
	uint8_t color_red, uint8_t color_green, uint8_t color_blue,
	int32_t canvas_size_width, int32_t canvas_size_height)
{
	if (point_x < 0 ||
		point_y < 0 ||
		point_x >= canvas_size_width ||
		point_y >= canvas_size_height)
	{
		return;
	}
	int index = (480 - point_y) * canvas_size_width * 3 + point_x * 3;
	pixel_data[index] = color_red;
	pixel_data[index + 1] = color_green;
	pixel_data[index + 2] = color_blue;
}

void set_pixel_rgb_protect_alpha(uint8_t *pixel_data,
	int32_t point_x, int32_t point_y,
	uint8_t color_red, uint8_t color_green, uint8_t color_blue,
	int32_t canvas_size_width, int32_t canvas_size_height,
	int32_t protect_alpha)
{
	//点を打つ アルファ付き（透明度を保護する
	if (point_x<0 || point_y<0 || point_x >= canvas_size_width || point_y >= canvas_size_height) {
		return;
	}

	color_t color = get_pixel_color(pixel_data, point_x, point_y, canvas_size_width, canvas_size_height);

	double dstA, srcA, newA;

	srcA = ((double)protect_alpha) / 255.0;
	dstA = ((double)color.alpha) / 255.0;
	newA = srcA + dstA - srcA * dstA;

	color.alpha = (uint8_t)(newA * 255 + 0.5);

	if (color.alpha) {
		color.red = (uint8_t)((color_red * srcA + color.red * dstA * (1 - srcA)) / newA + 0.5);
		color.green = (uint8_t)((color_green * srcA + color.green * dstA * (1 - srcA)) / newA + 0.5);
		color.blue = (uint8_t)((color_blue * srcA + color.blue * dstA * (1 - srcA)) / newA + 0.5);
	}

	int index = (canvas_size_height - point_y) * canvas_size_width * 3 + point_x * 3;

	pixel_data[index] = color.red;
	pixel_data[index + 1] = color.green;
	pixel_data[index + 2] = color.blue;
}

void set_line_rgb(uint8_t *pixel_data,
	int32_t start_point_x, int32_t start_point_y,
	int32_t end_point_x, int32_t end_point_y,
	uint8_t color_red, uint8_t color_green, uint8_t color_blue,
	int32_t canvas_size_width, int32_t canvas_size_height)
{
	//let
	int deltax = abs(end_point_x - start_point_x);
	int deltay = abs(end_point_y - start_point_y);
	int xstep = (end_point_x > start_point_x) ? 1 : -1;
	int ystep = (end_point_y > start_point_y) ? 1 : -1;
	//var
	int x = start_point_x;
	int y = start_point_y;
	int error;
	if (deltax >= deltay) {
		error = 2 * deltay - deltax;
		for (int i = 0; i <= deltax; ++i) {
			if (x<0 || x>canvas_size_width || y<0 || y>canvas_size_height) {} //描画範囲を超える場合
			else {
				set_pixel_rgb(pixel_data,
					x, y,
					color_red, color_green, color_blue,
					canvas_size_width, canvas_size_height);
			}
			x += xstep;
			error += 2 * deltay;
			if (error >= 0) {
				y += ystep;
				error -= 2 * deltax;
			}
		}
	}
	else {
		error = 2 * deltax - deltay;
		for (int i = 0; i <= deltay; ++i) {
			if (x<0 || x>canvas_size_width || y<0 || y>canvas_size_height) {}
			else {
				set_pixel_rgb(pixel_data,
					x, y,
					color_red, color_green, color_blue,
					canvas_size_width, canvas_size_height);
			}
			y += ystep;
			error += 2 * deltax;
			if (error >= 0) {
				x += xstep;
				error -= 2 * deltay;
			}
		}
	}
}

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

void fill_rect_rgb_slow(uint8_t *pixel_data,
	int32_t start_point_x, int32_t start_point_y,
	int32_t end_point_x, int32_t end_point_y,
	uint8_t color_red, uint8_t color_green, uint8_t color_blue,
	int32_t canvas_size_width, int32_t canvas_size_height)
{
	//普通に描画する方法：若干遅い
	int minx = MIN(start_point_x, end_point_x);
	int miny = MIN(start_point_y, end_point_y);
	int diffx = MAX(start_point_x, end_point_x) - MIN(start_point_x, end_point_x) + 1;
	int diffy = MAX(start_point_y, end_point_y) - MIN(start_point_y, end_point_y) + 1;
	for (int y = 0; y<diffy; y++) {
		for (int x = 0; x<diffx; x++) {
			set_pixel_rgb(pixel_data,
				minx + x, miny + y,
				color_red, color_green, color_blue,
				canvas_size_width, canvas_size_height);
		}
	}
}

void fill_circle_rgb(uint8_t *pixel_data,
	int32_t start_point_x, int32_t start_point_y,
	int32_t end_point_x, int32_t end_point_y,
	uint8_t color_red, uint8_t color_green, uint8_t color_blue,
	int32_t canvas_size_width, int32_t canvas_size_height)
{
	if (start_point_x >= end_point_x || start_point_y >= end_point_y) return;

	double _x1 = start_point_x;
	double _y1 = start_point_y;
	double _x2 = end_point_x;
	double _y2 = end_point_y;

	//幅の半径を求める
	double widthRadius = (_x2 - _x1) / 2.0;
	double heightRadius = (_y2 - _y1) / 2.0;//widthRadius / ratioHeight;
	double x, y;
	double ratioHeightReverse = (_y2 - _y1) / (_x2 - _x1);

	for (int iy = start_point_y; iy < end_point_y + heightRadius; iy++)
	{
		for (int ix = start_point_x; ix < end_point_x + widthRadius; ix++)
		{
			x = ix - start_point_x - widthRadius;
			y = iy - start_point_y - heightRadius;
			y /= ratioHeightReverse;
			if (x * x + y * y < widthRadius * widthRadius) {
				set_pixel_rgb(pixel_data,
					ix, iy,
					color_red, color_green, color_blue,
					canvas_size_width, canvas_size_height);
			}
		}
	}
}

#ifdef __HSPEXT__
typedef struct {
	int32_t width;
	int32_t height;
} image_size_t;

image_size_t
load_image(char const *file_name,
	uint8_t *pixel_data,
	int32_t point_x, int32_t point_y,
	int32_t canvas_size_width, int32_t canvas_size_height)
{
	uint8_t *image_pixels;
	int image_width;
	int image_height;
	int image_bpp;//色数 3 or 4
	image_pixels = stbi_load(file_name,
		&image_width,
		&image_height,
		&image_bpp,
		0);
	if (image_pixels == NULL) {
		raise_error("picload: image file could not be opened [%s]", file_name);
		exit(-1);
	}
	image_size_t image_size;
	image_size.width = image_width;
	image_size.height = image_height;
	int i = 0;
	for (int y = 0; y < image_height; y++) {
		for (int x = 0; x < image_width; x++) {
			if (image_bpp == 3) {
				int r = image_pixels[i];
				int g = image_pixels[i + 1];
				int b = image_pixels[i + 2];
				set_pixel_rgb(pixel_data,
					current_pos_x + x, current_pos_y + y,
					r, g, b,
					screen_width, screen_height);
				i += 3;
			}
			else {
				int r = image_pixels[i];
				int g = image_pixels[i + 1];
				int b = image_pixels[i + 2];
				int a = image_pixels[i + 3];
				set_pixel_rgb_protect_alpha(pixel_data,
					current_pos_x + x, current_pos_y + y,
					r, g, b,
					screen_width, screen_height,
					a);
				i += 4;
			}
		}
	}
	free(image_pixels);
	return image_size;
}
#endif

void redraw()
{
	// 描画の準備
	glClear(GL_COLOR_BUFFER_BIT);
	glRasterPos2i(-1, -1);

//Retina暫定対応
#ifdef __MACOS__

	int samplesPerPixel = 3;
	int now_width, now_height;
	glfwGetFramebufferSize(window, &now_width, &now_height);

	if (now_width > 640) {
		int width = screen_width;
		int height = screen_height;
		int h_mul = width * 2 * samplesPerPixel;
		uint8_t* retina_pixel_data = calloc(width * 2 * height * 2 * samplesPerPixel * 4, sizeof(uint8_t));
		int i = 0;
		for (int y = 0; y < height * 2; y += 2) {
			for (int x = 0; x < width * 2 * samplesPerPixel; x += 6) {
				memcpy(&retina_pixel_data[x + y * h_mul], &pixel_data[i], sizeof(uint8_t) * 3);
				memcpy(&retina_pixel_data[x + 3 + h_mul * y], &pixel_data[i], sizeof(uint8_t) * 3);
				memcpy(&retina_pixel_data[x + h_mul * (y + 1)], &pixel_data[i], sizeof(uint8_t) * 3);
				memcpy(&retina_pixel_data[x + 3 + h_mul * (y + 1)], &pixel_data[i], sizeof(uint8_t) * 3);
				i += 3;
			}
		}
		glDrawPixels(width * 2, height * 2, GL_RGB, GL_UNSIGNED_BYTE, retina_pixel_data);
		free(retina_pixel_data);
	}
	else

#endif

	{
		// ピクセルを描画
		glDrawPixels(screen_width, screen_height, GL_RGB, GL_UNSIGNED_BYTE, pixel_data);
	}
	// フロントバッファとバックバッファを交換する
	glfwSwapBuffers(window);
}

// 文字列操作
void
init_str(char* str, int size)
{
	for (int i = 0; i < size; i++) {
		str[i] = '\0';
	}
}

int
utf8len(uint8_t c)
{
	if (c >= 0 && c < 128) { return 1; }
	if (c >= 128 && c < 192) { return 2; }
	if (c >= 192 && c < 224) { return 2; }
	if (c >= 224 && c < 240) { return 3; }
	if (c >= 240 && c < 248) { return 4; }
	if (c >= 248 && c < 252) { return 5; }
	if (c >= 252 && c < 254) { return 6; }
	if (c >= 254 && c <= 255) { return 6; }
	return 0;
}

int
utf8strlen(const char* s)
{
	int length = 0;
	for (int i = 0; i < (int)strlen(s); i++) {
		if (s[i] == '\0') {
			break;
		}
		i += utf8len(s[i]) - 1;
		length++;
	}
	return length;
}

void
utf8char_at(char* dest, const char* src, int index)
{
	int count = 0;
	for (int i = 0; i < (int)strlen(src); i++) {
		if (src[i] == '\0') {
			break;
		}
		if (count == index) {
			for (int j = 0; j < utf8len(src[i]); j++) {
				dest[j] = src[i + j];
			}
			return;
		}
		i += utf8len(src[i]) - 1;
		count++;
	}
	return;
}

uint32_t
utf8_to_utf32(const uint8_t* input)
{
	uint8_t c1, c2, c3, c4, c5, c6;
	if (!input) {
		return 0;
	}
	c1 = input[0];
	if ((c1 & 0x80) == 0x00) {
		return c1;
	}
	if ((c1 & 0xc0) == 0x80) {
		return 0;
	}
	if (c1 == 0xfe || c1 == 0xFF) {
		return 0;
	}
	c2 = input[1];
	if (((c1 & 0xe0) == 0xc0) &&
		((c2 & 0xc0) == 0x80))
	{
		return ((c1 & 0x1f) << 6) | (c2 & 0x3f);
	}
	c3 = input[2];
	if (((c1 & 0xf0) == 0xe0) &&
		((c2 & 0xc0) == 0x80) &&
		((c3 & 0xc0) == 0x80))
	{
		return ((c1 & 0x0f) << 12) | ((c2 & 0x3f) << 6) | (c3 & 0x3f);
	}
	c4 = input[3];
	if (((c1 & 0xf8) == 0xf0) &&
		((c2 & 0xc0) == 0x80) &&
		((c3 & 0xc0) == 0x80) &&
		((c4 & 0xc0) == 0x80))
	{
		return ((c1 & 0x07) << 18) | ((c2 & 0x3f) << 12) | ((c3 & 0x3f) << 6) | (c4 & 0x3f);
	}
	c5 = input[4];
	if (((c1 & 0xfc) == 0xf0) &&
		((c2 & 0xc0) == 0x80) &&
		((c3 & 0xc0) == 0x80) &&
		((c4 & 0xc0) == 0x80) &&
		((c5 & 0xc0) == 0x80))
	{
		return ((c1 & 0x03) << 24) | ((c2 & 0x3f) << 18) | ((c3 & 0x3f) << 12) | ((c4 & 0x3f) << 6) | (c5 & 0x3f);
	}
	c6 = input[5];
	if (((c1 & 0xfe) == 0xf0) &&
		((c2 & 0xc0) == 0x80) &&
		((c3 & 0xc0) == 0x80) &&
		((c4 & 0xc0) == 0x80) &&
		((c5 & 0xc0) == 0x80) &&
		((c6 & 0xc0) == 0x80))
	{
		return ((c1 & 0x01) << 30) | ((c2 & 0x3f) << 24) | ((c3 & 0x3f) << 18) | ((c4 & 0x3f) << 12) | ((c5 & 0x3f) << 6) | (c6 & 0x3f);
	}
	return 0;
}

// 乱数
unsigned long xor128()
{
	static unsigned long x = 123456789, y = 362436069, z = 521288629, w = 88675123;
	unsigned long t;
	t = (x ^ (x << 11)); x = y; y = z; z = w; return(w = (w ^ (w >> 19)) ^ (t ^ (t >> 8)));
}

double
randf(double min, double max)
{
	return ((max - min)*((float)xor128() / 0x100000000)) + min;
}

// 一の位で四捨五入
int round_one(int n) {
	double tmp_d = (double)n;
	if (tmp_d < 0) {
		tmp_d -= 5;
	}
	else {
		tmp_d += 5;
	}
	tmp_d /= 10;
	tmp_d = floor(tmp_d);
	tmp_d *= 10;
	return (int)tmp_d;
}
#endif

//========================================================

#define NHSP_UNUA(v)
#define NHSP_UNUSE(v) ((void)v)

// 内部リンケージの人たち、ここから
#if NHSP_CONFIG_MEMLEAK_DETECTION
static list_t* s_memory_map_ = nullptr;
#endif

// メモリ
void*
zmalloc(size_t size)
{
	return malloc(size);
}

void
zfree(void* ptr)
{
	free(ptr);
}

void*
zrealloc(void* ptr, size_t size)
{
	return realloc(ptr, size);
}

void
zregister_memory(void* ptr)
{
#if NHSP_CONFIG_MEMLEAK_DETECTION
	assert(s_memory_map_ != nullptr);
	const auto node =
		reinterpret_cast<list_node_t*>(zmalloc(sizeof(list_node_t)));
	node->next_ = node->prev_ = nullptr;
	node->value_ = ptr;
	list_append(*s_memory_map_, node);
#else
	NHSP_UNUSE(ptr);
#endif
}

void
zunregister_memory(void* ptr)
{
#if NHSP_CONFIG_MEMLEAK_DETECTION
	assert(s_memory_map_ != nullptr);
	const auto node = list_find(*s_memory_map_, ptr);
	assert(node != nullptr);
	list_erase(*s_memory_map_, node);
	zfree(node);
#else
	NHSP_UNUSE(ptr);
#endif
}

// 文字列
char*
create_string(size_t len)
{
	return (char*)xmalloc(len + 1, NULL);
}

char*
create_string2(const char* s, size_t len)
{
	char* res = create_string(len);
	strncpy(res, s, len);
	res[len] = '\0';
	return res;
}

char*
create_string3(const char* s)
{
	if (s == NULL) {
		return create_string2("", 0);
	}
	else {
		return create_string2(s, strlen(s));
	}
}

void
destroy_string(char* s)
{
	xfree(s);
}

char*
create_string_from(int v)
{
	const size_t len = (size_t)snprintf(NULL, 0, "%d", v);
	char* res = create_string(len);
	sprintf(res, "%d", v);
	return res;
}

char*
create_string_from2(double v)
{
	const size_t len = (size_t)snprintf(NULL, 0, "%lf", v);
	char* res = create_string(len);
	sprintf(res, "%lf", v);
	return res;
}

const int
tol(char c) {
	if (c >= 'A' && c <= 'Z') {
		return c - 'A' + 'a';
	}
	return c;
}

bool
string_equal_igcase(const char* sl, const char* r, int len/* = -1*/) //※ 引数の初期値
{
	int iter = 0;
	for (;;) {
		if (sl[0] == '\0' || r[0] == '\0') {
			if (len < 0) {
				if (sl[0] != r[0]) {
					return false;
				}
			}
			else {
				if (sl[0] != '\0' || iter != len) {
					return false;
				}
			}
			break;
		}
		if (tol(sl[0]) != tol(r[0])) {
			return false;
		}
		++sl, ++r;
		++iter;
	}
	return true;
}

// 値
void
clear_value(value_t* t)
{
	switch (t->type_) {
	case VALUE_STRING:
		destroy_string(t->svalue_);
		break;
	default:
		break;
	}
}

// 実行環境ユーティリティ
list_node_t*
search_label(execute_environment_t* e, const char* name)
{
	list_node_t* node = e->label_table_->head_;
	while (node != NULL) {
		const label_node_t* label = (label_node_t*)node->value_;
		if (string_equal_igcase(label->name_, name, -1)) {
			return label->statement_;
		}
		node = node->next_;
	}
	return NULL;
}

// コマンド実体
void
command_dim(execute_environment_t* e, execute_status_t* s, int arg_num)
{
	if (arg_num != 2) {
		raise_error("dim: Array variable is one dimension only.");
	}
	const int arg_start = -arg_num;
	const value_t* v = stack_peek(s->stack_, arg_start);
	if (v->type_ != VALUE_VARIABLE) {
		raise_error("dim: Argument should be a variable.");
	}
	if (v->index_ > 0) {
		raise_error("dim: Array variables cannot be specified.");
	}
	const value_t* n = stack_peek(s->stack_, arg_start + 1);
	const int num = value_calc_int(n);
	if (num <= 0) {
		raise_error("dim: Invalid value.");
	}
	prepare_variable(v->variable_, VALUE_INT, 64, num);
	stack_pop(s->stack_, arg_num);
}

void
command_ddim(execute_environment_t* e, execute_status_t* s, int arg_num)
{
	if (arg_num != 2) {
		raise_error("ddim: Array variable is one dimension only.");
	}
	const int arg_start = -arg_num;
	const value_t* v = stack_peek(s->stack_, arg_start);
	if (v->type_ != VALUE_VARIABLE) {
		raise_error("ddim: Argument should be a variable.");
	}
	if (v->index_ > 0) {
		raise_error("ddim: Array variables cannot be specified.");
	}
	const value_t* n = stack_peek(s->stack_, arg_start + 1);
	const int num = value_calc_int(n);
	if (num <= 0) {
		raise_error("ddim: Invalid value.");
	}
	prepare_variable(v->variable_, VALUE_DOUBLE, 64, num);
	stack_pop(s->stack_, arg_num);
}

void
command_sdim(execute_environment_t* e, execute_status_t* s, int arg_num)
{
	if (arg_num < 2 || arg_num > 3) {
		raise_error("sdim: Array variable is one dimension only.");
	}
	const int arg_start = -arg_num;
	const value_t* v = stack_peek(s->stack_, arg_start);
	if (v->type_ != VALUE_VARIABLE) {
		raise_error("sdim: Argument should be a variable.");
	}
	if (v->index_ > 0) {
		raise_error("ddim: Array variables cannot be specified.");
	}
	const value_t* g = stack_peek(s->stack_, arg_start + 1);
	const int granule = value_calc_int(g);
	const int num = (arg_num > 2 ? value_calc_int(stack_peek(s->stack_, arg_start + 2)) : 1);
	if (granule <= 0 || num <= 0) {
		raise_error("ddim: Invalid value.");
	}
	prepare_variable(v->variable_, VALUE_STRING, granule, num);
	stack_pop(s->stack_, arg_num);
}

#ifdef __HSPCUI__
void
command_mes(execute_environment_t* e, execute_status_t* s, int arg_num)
{
	if (arg_num != 1) {
		raise_error("mes: Invalid argument.");
	}
	value_t* m = stack_peek(s->stack_, -1);
	value_isolate(m);
	if (m->type_ != VALUE_STRING) {
		raise_error("mes: Invalid value.");
	}
	printf("%s\n", m->svalue_);
	stack_pop(s->stack_, arg_num);
}
#endif

#ifdef __HSPEXT__
void
command_mes(execute_environment_t* e, execute_status_t* s, int arg_num)
{

	if (arg_num != 1) {
		raise_error("mes: Invalid argument.");
	}
	value_t* m = stack_peek(s->stack_, -1);
	value_isolate(m);
	if (m->type_ != VALUE_STRING) {
		raise_error("mes: Invalid value.");
	}

	// 文字列を描画
	{
		char* src_str = m->svalue_;
		char dest_str[8];
		int codepoint_utf32;
		int pos_x = 0; // 文字表示のX座標
		float xpos = 0.0;
		int font_buffer_h = 128;
		int font_buffer_w = 128;
		int out_stride = 128;
		unsigned char font_buffer[128][128];
		for (int i = 0; i < utf8strlen(src_str); i++) {
			// 文字を取得してutf16に変換
			init_str(dest_str, 8); // 取得用の変数を初期化 
			utf8char_at(dest_str, src_str, i); // 任意の位置の１文字を取得
			codepoint_utf32 = (int)utf8_to_utf32((uint8_t*)dest_str); //utf8をutf32に変換
																	  // 空白文字の処理
																	  // 0x200B // ゼロ幅空白
			if (codepoint_utf32 == 0x20 || // 半角スペース
				codepoint_utf32 == 0xA0 || // 改行禁止スペース
				codepoint_utf32 == 0x2002 || // n幅空白
				codepoint_utf32 == 0x2003 || // m幅空白
				codepoint_utf32 == 0x2004 || // 1/3
				codepoint_utf32 == 0x2005 || // 1/4
				codepoint_utf32 == 0x2009 // 狭い空白
				)
			{
				pos_x += (int)((float)font_size * font_half_space_ratio);
				continue;
			}
			else if (codepoint_utf32 == 0x3000) { // 全角空白
				pos_x += (int)((float)font_size * font_full_space_ratio);
				continue;
			}
			// font_bufferを初期化
			for (int y = 0; y < font_buffer_h; y++) {
				for (int x = 0; x < font_buffer_w; x++) {
					font_buffer[x][y] = 0;
				}
			}
			// フォントをレンダリングする
			int font_advance = 0;
			int lsb = 0;
			int x0, y0, x1, y1 = 0;
			float x_shift = xpos - (float)floor(xpos);

			stbtt_GetCodepointHMetrics(&font, codepoint_utf32, &font_advance, &lsb);

			stbtt_GetCodepointBitmapBoxSubpixel(&font, codepoint_utf32,
				font_scale, font_scale,
				x_shift, 0,
				&x0, &y0, &x1, &y1);
			stbtt_MakeCodepointBitmapSubpixel(&font, // info
				&font_buffer[font_baseline + y0][(int)xpos + x0], // output
				x1 - x0, // out_w
				y1 - y0, // out_h
				out_stride, // out_stride
				font_scale, font_scale, x_shift, 0,
				codepoint_utf32);
			// １文字分を描画
			for (int y = 0; y < font_buffer_h; ++y) {
				for (int x = 0; x < font_buffer_w; ++x) {
					if (font_smooth) {
						set_pixel_rgb_protect_alpha(pixel_data,
							current_pos_x + pos_x + x, current_pos_y + y,
							current_color_r, current_color_g, current_color_b,
							screen_width, screen_height,
							font_buffer[y][x]);
					}
					else {
						if (font_buffer[y][x] >= font_threshold) {
							set_pixel_rgb(pixel_data,
								current_pos_x + pos_x + x, current_pos_y + y,
								current_color_r, current_color_g, current_color_b,
								screen_width, screen_height);
						}
					}
				}
			}
			pos_x += x1;
		}
		current_pos_y += font_size;
	}
	if (redraw_flag) {
		redraw();
	}
	stack_pop(s->stack_, arg_num);
}
#endif

#ifdef __HSPCUI__
void
command_input(execute_environment_t* e, execute_status_t* s, int arg_num)
{
	if (arg_num < 2 || arg_num > 3) {
		raise_error("input: Invalid argument.");
	}
	const int arg_start = -arg_num;
	const value_t* v = stack_peek(s->stack_, arg_start);
	if (v->type_ != VALUE_VARIABLE) {
		raise_error("input: Argument should be a variable.");
	}
	if (v->index_ > 0) {
		raise_error("input: Array variables cannot be specified.");
	}
	const value_t* n = stack_peek(s->stack_, arg_start + 1);
	const int len = value_calc_int(n) + 1;
	const int mode =
		(arg_num > 2 ? value_calc_int(stack_peek(s->stack_, arg_start + 2)) : 0);
	char* buf = create_string(len);
	int w = 0;
	for (;;) {
		if (w >= len) {
			break;
		}
		const char c = getchar();
		if (c == EOF) {
			break;
		}
		const char ch = (char)c;
		if (mode == 1 && ch == '\n') {
			break;
		}
		if (mode == 2) {
			if (ch == '\r') {
				const char nc = getchar();
				if ((char)nc == '\n') {
					break;
				}
				ungetc(nc, stdin);
			}
			else if (ch == '\n') {
				break;
			}
		}
		buf[w] = (char)ch;
		++w;
	}
	buf[w] = '\0';
	value_t* t = (value_t*)create_value_move(buf);
	variable_set(e->variable_table_, t, v->variable_->name_, 0);
	destroy_value((value_t*)t);
	s->strsize_ = w;
	stack_pop(s->stack_, arg_num);
}
#endif

void
command_bload(execute_environment_t* e, execute_status_t* s, int arg_num)
{
	// 引数の数をチェックする
	if (arg_num != 2) {
		raise_error("bload: Invalid argument.");
	}
	const int arg_start = -arg_num;

	// １つ目の引数を取得する
	value_t* m = stack_peek(s->stack_, arg_start);
	value_isolate(m);
	if (m->type_ != VALUE_STRING) {
		raise_error("bload: Argument should specify a string type.");
	}

	char* name = m->svalue_;
	// ２つめの引数を取得する
	const value_t* v = stack_peek(s->stack_, arg_start + 1);
	if (v->type_ != VALUE_VARIABLE) {
		raise_error("bload: Argument should be a variable.");
	}

	// ファイルをオープンする
	FILE* fp = fopen(name, "rb");
	if (fp == NULL) {
		raise_error("bload: file could not be opened [%s]", name);
		exit(1);
	}
	// ファイルサイズを取得する
	fseek(fp, 0, SEEK_END); // ファイル位置表示子をファイルの最後に位置付ける
	long size = ftell(fp);

	// バイナリを一時的な変数に読み込む
	fseek(fp, 0, SEEK_SET); // ファイル位置表示子をファイルの始めに位置付ける
	uint8_t* tmp = (uint8_t*)calloc(size, sizeof(uint8_t));
	fread(tmp, sizeof(uint8_t), (int)size, fp);

	// 指定された変数に一時的な変数から代入する
	void* data_ptr = v->variable_->data_;
	uint8_t* data_ptr_u8 = (uint8_t*)data_ptr;
	for (int i = 0; i < size; i++) {
		data_ptr_u8[i] = tmp[i];
	}

	// システム変数にファイルサイズを書き込む
	s->strsize_ = size;

	// 解放処理
	free(tmp);
	fclose(fp);
	stack_pop(s->stack_, arg_num);
}

void
command_bsave(execute_environment_t* e, execute_status_t* s, int arg_num)
{
	// 引数の数をチェックする
	if (arg_num != 2) {
		raise_error("bsave: Invalid argument.");
	}
	const int arg_start = -arg_num;

	// １つ目の引数を取得する
	value_t* m = stack_peek(s->stack_, arg_start);
	value_isolate(m);
	if (m->type_ != VALUE_STRING) {
		raise_error("bsave: Argument should specify a string type.");
	}
	char* fnm = m->svalue_;

	// ２つめの引数を取得する
	const value_t* v = stack_peek(s->stack_, arg_start + 1);
	if (v->type_ != VALUE_VARIABLE) {
		raise_error("bsave: Argument should be a variable.");
	}

	// ファイルをオープンする
	FILE* fp = fopen(fnm, "wb");
	if (fp == NULL) {
		raise_error("bsave: file could not be opened [%s]", fnm);
		exit(1);
	}

	// 指定した変数の内容をdata_ptrに代入する
	void* data_ptr = v->variable_->data_;

	// データサイズを取得する
	const long size = strlen(data_ptr);

	// ファイルにdata_ptrの内容を書き込む
	fwrite(data_ptr, size, 1, fp);

	// 書き込んだサイズをシステム変数に代入
	s->strsize_ = size;

	// クローズ処理
	fclose(fp);
	stack_pop(s->stack_, arg_num);
}

void
command_poke(execute_environment_t* e, execute_status_t* s, int arg_num)
{
	// 引数の数をチェックする
	if (arg_num != 3) {
		raise_error("poke: Invalid argument.");
	}
	const int arg_start = -arg_num;
	// １つ目の引数を取得する
	const value_t* v = stack_peek(s->stack_, arg_start);
	if (v->type_ != VALUE_VARIABLE) {
		raise_error("poke: Argument should be a variable.");
	}
	// ２つめの引数を取得する
	const value_t* p2 = stack_peek(s->stack_, arg_start + 1);
	const int i = value_calc_int(p2);
	// ３つめの引数を取得する
	const value_t* p3 = stack_peek(s->stack_, arg_start + 2);
	const int num = value_calc_int(p3);

	// 指定された変数に代入
	void* data_ptr = v->variable_->data_;
	char* tmp = (char*)data_ptr;
	tmp[i] = num;
	stack_pop(s->stack_, arg_num);
}

void
command_run(execute_environment_t* e, execute_status_t* s, int arg_num)
{
	if (arg_num != 1) {
		raise_error("run: Invalid argument.");
	}
	const value_t* m = stack_peek(s->stack_, -1);
	if (m->type_ != VALUE_STRING) {
		raise_error("run: Argument should specify a string type.");
	}
	filename = value_calc_string(m);
	run_flag = true;
	s->is_end_ = true;
	stack_pop(s->stack_, arg_num);
}

#ifdef __HSPGUI__
void
command_wait(execute_environment_t* e, execute_status_t* s, int arg_num)
{
	if (arg_num != 1) {
		raise_error("wait: Invalid argument.");
	}
	double wait_time = 0.0;
	const value_t* m = stack_peek(s->stack_, -1);
	wait_time = value_calc_double(m);
	glfwSetTime(0.0); // タイマーを初期化する
	for (;;) { // ウィンドウを閉じるまで
		if (glfwWindowShouldClose(window)) {
			s->is_end_ = true;
			break;
		}
		if (glfwGetTime() * 100 > wait_time) {
			break;
		}
		glfwPollEvents(); // イベント待ち
	}
	stack_pop(s->stack_, arg_num);
}

void
command_stop(execute_environment_t* e, execute_status_t* s, int arg_num)
{
	if (arg_num >= 1) {
		raise_error("stop: Invalid argument.");
	}
	for (;;) { // ウィンドウを閉じるまで
		if (glfwWindowShouldClose(window)) {
			s->is_end_ = true;
			break;
		}
		glfwPollEvents(); // イベント待ち
	}
	stack_pop(s->stack_, arg_num);
}

void
command_title(execute_environment_t* e, execute_status_t* s, int arg_num)
{
	if (arg_num != 1) {
		raise_error("title: Invalid argument.");
	}
	value_t* m = stack_peek(s->stack_, -1);
	value_isolate(m);
	if (m->type_ != VALUE_STRING) {
		raise_error("title: Argument should specify a string type.");
	}
	glfwSetWindowTitle(window, m->svalue_);
	stack_pop(s->stack_, arg_num);
}

void
command_pset(execute_environment_t* e, execute_status_t* s, int arg_num)
{
	if (arg_num > 2) {
		raise_error("pset: Invalid argument.");
	}
	if (arg_num <= 0) { // 引数が省略されば場合
		set_pixel_rgb(pixel_data,
			current_pos_x, current_pos_y,
			current_color_r, current_color_g, current_color_b,
			screen_width, screen_height);
	}
	else {
		const int arg_start = -arg_num;
		const value_t* p1 = stack_peek(s->stack_, arg_start);
		const int x = value_calc_int(p1);
		const value_t* p2 = stack_peek(s->stack_, arg_start + 1);
		const int y = value_calc_int(p2);
		set_pixel_rgb(pixel_data,
			x, y,
			current_color_r, current_color_g, current_color_b,
			screen_width, screen_height);
	}
	if (redraw_flag) {
		redraw();
	}
	stack_pop(s->stack_, arg_num);
}

void
command_redraw(execute_environment_t* e, execute_status_t* s, int arg_num)
{
	if (arg_num > 1) {
		raise_error("redraw: Invalid argument.");
	}
	if (arg_num <= 0) { // 引数が省略された
		redraw_flag = true;
		redraw();
	}
	int redraw_flag_num = 0;
	const value_t* n = stack_peek(s->stack_, -1);
	redraw_flag_num = value_calc_int(n);
	if (redraw_flag_num == 0) {
		redraw_flag = false;
	}
	else if (redraw_flag_num == 1) {
		redraw_flag = true;
		redraw();
	}
	else {
		raise_error("redraw: Argument values only 0 or 1.");
	}
	stack_pop(s->stack_, arg_num);
}

void
command_pos(execute_environment_t* e, execute_status_t* s, int arg_num)
{
	if (arg_num != 2) {
		raise_error("pos: Invalid argument.");
	}
	const int arg_start = -arg_num;
	const value_t* p1 = stack_peek(s->stack_, arg_start);
	const int x = value_calc_int(p1);
	const value_t* p2 = stack_peek(s->stack_, arg_start + 1);
	const int y = value_calc_int(p2);
	current_pos_x = x;
	current_pos_y = y;
	stack_pop(s->stack_, arg_num);
}

void
command_color(execute_environment_t* e, execute_status_t* s, int arg_num)
{
	if (arg_num > 3) {
		raise_error("color: Invalid argument.");
	}
	if (arg_num <= 0) { // 引数が省略された場合
		current_color_r = 0;
		current_color_g = 0;
		current_color_b = 0;
	}
	else {
		const int arg_start = -arg_num;
		const value_t* p1 = stack_peek(s->stack_, arg_start);
		const int r = value_calc_int(p1);
		const value_t* p2 = stack_peek(s->stack_, arg_start + 1);
		const int g = value_calc_int(p2);
		const value_t* p3 = stack_peek(s->stack_, arg_start + 2);
		const int b = value_calc_int(p3);
		current_color_r = r;
		current_color_g = g;
		current_color_b = b;
	}
	stack_pop(s->stack_, arg_num);
}

void
command_line(execute_environment_t* e, execute_status_t* s, int arg_num)
{
	if (arg_num != 4) {
		raise_error("line: Invalid argument.");
	}
	const int arg_start = -arg_num;
	const value_t* p1 = stack_peek(s->stack_, arg_start);
	const int sx = value_calc_int(p1);
	const value_t* p2 = stack_peek(s->stack_, arg_start + 1);
	const int sy = value_calc_int(p2);
	const value_t* p3 = stack_peek(s->stack_, arg_start + 2);
	const int ex = value_calc_int(p3);
	const value_t* p4 = stack_peek(s->stack_, arg_start + 3);
	const int ey = value_calc_int(p4);
	set_line_rgb(pixel_data,
		sx, sy,
		ex, ey,
		current_color_r, current_color_g, current_color_b,
		screen_width, screen_height);
	if (redraw_flag) {
		redraw();
	}
	stack_pop(s->stack_, arg_num);
}

void
command_boxf(execute_environment_t* e, execute_status_t* s, int arg_num)
{
	if (arg_num != 4) {
		raise_error("boxf: Invalid argument.");
	}
	const int arg_start = -arg_num;
	const value_t* p1 = stack_peek(s->stack_, arg_start);
	const int x0 = value_calc_int(p1);
	const value_t* p2 = stack_peek(s->stack_, arg_start + 1);
	const int y0 = value_calc_int(p2);
	const value_t* p3 = stack_peek(s->stack_, arg_start + 2);
	const int x1 = value_calc_int(p3);
	const value_t* p4 = stack_peek(s->stack_, arg_start + 3);
	const int y1 = value_calc_int(p4);
	fill_rect_rgb_slow(pixel_data,
		x0, y0,
		x1, y1,
		current_color_r, current_color_g, current_color_b,
		screen_width, screen_height);
	if (redraw_flag) {
		redraw();
	}
	stack_pop(s->stack_, arg_num);
}

void
command_stick(execute_environment_t* e, execute_status_t* s, int arg_num)
{
	if (arg_num < 1 || arg_num > 2) {
		raise_error("stick: Invalid argument.");
	}
	const int arg_start = -arg_num;
	const value_t* v = stack_peek(s->stack_, arg_start);
	if (v->type_ != VALUE_VARIABLE) {
		raise_error("stick: Argument should be a variable.");
	}
	if (v->index_ > 0) {
		raise_error("stick: Array variables cannot be specified.");
	}

	int key = 0;
	if (glfwGetKey(window, GLFW_KEY_LEFT)) {
		key += 1;
	}
	if (glfwGetKey(window, GLFW_KEY_UP)) {
		key += 2;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT)) {
		key += 4;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN)) {
		key += 8;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE)) {
		key += 16;
	}
	if (glfwGetKey(window, GLFW_KEY_ENTER)) {
		key += 32;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) || glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL)) {
		key += 64;
	}
	if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
		key += 128;
	}
	if (current_mouse_down_left == 1) {
		key += 256;
	}
	if (current_mouse_down_right == 1) {
		key += 512;
	}
	if (glfwGetKey(window, GLFW_KEY_TAB)) {
		key += 1024;
	}

	// 指定された変数に代入
	void* data_ptr = v->variable_->data_;
	int* tmp = (int*)data_ptr;
	tmp[0] = key;
	stack_pop(s->stack_, arg_num);
}

void
command_circle(execute_environment_t* e, execute_status_t* s, int arg_num)
{
	if (arg_num < 4) {
		raise_error("circle: Invalid argument.");
	}
	const int arg_start = -arg_num;

	const value_t* p1 = stack_peek(s->stack_, arg_start);
	const int x0 = value_calc_int(p1);
	const value_t* p2 = stack_peek(s->stack_, arg_start + 1);
	const int y0 = value_calc_int(p2);
	const value_t* p3 = stack_peek(s->stack_, arg_start + 2);
	const int x1 = value_calc_int(p3);
	const value_t* p4 = stack_peek(s->stack_, arg_start + 3);
	const int y1 = value_calc_int(p4);

	fill_circle_rgb(
		pixel_data, x0, y0, x1, y1,
		current_color_r, current_color_g, current_color_b,
		screen_width, screen_height);

	if (redraw_flag) redraw();

	stack_pop(s->stack_, arg_num);
}

#ifdef __HSPEXT__
void
command_font(execute_environment_t* e, execute_status_t* s, int arg_num)
{
	if (arg_num > 3 || arg_num <= 0) {
		raise_error("font: Invalid argument.");
	}
	char* name = (char*)"";
	int size = font_size;
	int style = 16;
	const int arg_start = -arg_num;
	if (arg_num >= 1) {
		value_t* n = stack_peek(s->stack_, arg_start);
		value_isolate(n);
		if (n->type_ != VALUE_STRING) {
			raise_error("font: Invalid value.");
		}
		name = n->svalue_;
	}
	if (arg_num >= 2) {
		const value_t* p2 = stack_peek(s->stack_, arg_start + 1);
		size = value_calc_int(p2);
	}
	if (arg_num >= 3) {
		const value_t* p3 = stack_peek(s->stack_, arg_start + 2);
		style = value_calc_int(p3);
	}
	if (strcmp(name, "") != 0) {
		memset(font_ttf_buffer, 0, 2000000);
		FILE* fp = fopen(name, "rb");
		if (fp == NULL) {
			raise_error("font: Font %s cannot be opened.", name);
		}
		fread(font_ttf_buffer, 1, 2000000, fp);
		fclose(fp);
		int offset = stbtt_GetFontOffsetForIndex((unsigned char *)font_ttf_buffer, 0);
		stbtt_InitFont(&font, (unsigned char *)font_ttf_buffer, offset);
	}
	if (font_size != size) { // フォント情報の生成
		font_size = size;
		if (font_size > 100) {
			raise_error("font：Maximum font size is 100.");
		}
		font_scale = stbtt_ScaleForPixelHeight(&font, (float)font_size);
		stbtt_GetFontVMetrics(&font, &font_ascent, 0, 0);
		font_baseline = (int)(font_ascent * font_scale);
	}
	if (style == 16) {
		font_smooth = true;
	}
	else {
		font_smooth = false;
	}
	stack_pop(s->stack_, arg_num);
}

void
command_picload(execute_environment_t* e, execute_status_t* s, int arg_num)
{
	if (arg_num != 1) {
		raise_error("picload: Invalid argument.");
	}
	value_t* m = stack_peek(s->stack_, -1);
	value_isolate(m);
	if (m->type_ != VALUE_STRING) {
		raise_error("picload: Invalid value.");
	}
	// 画像を読み込む
	load_image(m->svalue_, pixel_data,
		current_pos_x, current_pos_y,
		screen_width, screen_height);
	stack_pop(s->stack_, arg_num);
}

void
command_beep(execute_environment_t* e, execute_status_t* s, int arg_num)
{
	int freq = 440;
	int duration = 1000;
	int waveform = 2; // 0 - 4
	int16_t volume = 3000;

	if (arg_num > 4) {
		raise_error("beep: Invalid argument.");
	}
	const int arg_start = -arg_num;

	// 引数が省略された場合
	if (arg_num > 3) {
		const value_t* p4 = stack_peek(s->stack_, arg_start + 3);
		volume = value_calc_int(p4);
	}
	if (arg_num > 2) {
		const value_t* p3 = stack_peek(s->stack_, arg_start + 2);
		waveform = value_calc_int(p3);
	}
	if (arg_num > 1) {
		const value_t* p2 = stack_peek(s->stack_, arg_start + 1);
		duration = value_calc_int(p2);
	}
	if (arg_num > 0) {
		const value_t* p1 = stack_peek(s->stack_, arg_start);
		freq = value_calc_int(p1);
	}

	// 音を生成して再生
	double f = freq;
	double fs = 44100.0;
	ALuint buffer;
	ALuint source;
	ALsizei size = round_one((int)fs * duration / 500);
	ALshort* data = (ALshort*)calloc(size, sizeof(ALshort));
	alGenBuffers(1, &buffer); // 次の行は音のデータを作成している

	// 音の生成
	double n = 0.0;
	double tmp = 0;
	for (int i = 0; i < size; i++) {
		switch (waveform) {
		case 0: // 正弦波
			tmp = volume * sin(2.0 * M_PI * f * n / fs);
			break;
		case 1: // ノコギリ波
		{
			double cons = fs / f;
			double cons2 = 2.0 * volume * f / fs;
			tmp = cons2 * fmod(n, cons) - volume;
			break;
		}
		case 2: // 矩形波
			if (fmod(n, fs / f) < fs / (2.0 * f)) {
				tmp = volume;
			}
			else {
				tmp = -volume;
			}
			break;
		case 3: // 三角波
			if (fmod(n, fs / f) < fs / (2.0 * f)) {
				tmp = volume * (2.0 * (f / fs) * fmod(n, fs / f) - 1.0);
			}
			else {
				tmp = -(volume * (2.0 * (f / fs) * fmod(n, fs / f) - 1.0));
			}
			break;
		case 4: // 白雑音
			tmp = volume * randf(-1, 1);
			break;
		}
		data[i] = (ALshort)tmp;
		n += 1.0;
	}

	alBufferData(buffer, AL_FORMAT_MONO16, data, size, 44100); // バッファにデータを格納
	alGenSources(1, &source); // ソースを生成
	alSourcei(source, AL_BUFFER, buffer); // ソースの値を設定
	alSourcePlay(source); // ソースのバッファを再生

	// 再生が終了するまで待つ
	glfwSetTime(0.0); // タイマーを初期化する
	for (;;) { // ウィンドウを閉じるまで
		if (glfwWindowShouldClose(window)) {
			s->is_end_ = true;
			break;
		}
		if (glfwGetTime() * 1000.0 > (double)duration) {
			break;
		}
		glfwPollEvents(); // イベント待ち
	}

	alSourceStop(source); // ソースのバッファを停止
	alDeleteSources(1, &source); // ソースを消去
	alDeleteBuffers(1, &buffer); //バッファを消去
	free(data);

	stack_pop(s->stack_, arg_num);
}
#endif
#endif

// 関数実体
void
function_int(execute_environment_t* e, execute_status_t* s, int arg_num)
{
	if (arg_num != 1) {
		raise_error("int: Invalid argument.");
	}
	const value_t* m = stack_peek(s->stack_, -1);
	const int r = value_calc_int(m);
	stack_pop(s->stack_, arg_num);
	stack_push(s->stack_, create_value(r));
}

void
function_double(execute_environment_t* e, execute_status_t* s, int arg_num)
{
	if (arg_num != 1) {
		raise_error("double: Invalid argument.");
	}
	const value_t* m = stack_peek(s->stack_, -1);
	int r = (int)value_calc_double(m);
	stack_pop(s->stack_, arg_num);
	stack_push(s->stack_, create_value2(r));
}

void
function_str(execute_environment_t* e, execute_status_t* s, int arg_num)
{
	if (arg_num != 1) {
		raise_error("str: Invalid argument.");
	}
	const value_t* m = stack_peek(s->stack_, -1);
	char* r = value_calc_string(m);
	stack_pop(s->stack_, arg_num);
	stack_push(s->stack_, create_value_move(r));
}

void
function_rnd(execute_environment_t* e, execute_status_t* s, int arg_num)
{
	if (arg_num != 1) {
		raise_error("rnd: Invalid argument.");
	}
	const value_t* m = stack_peek(s->stack_, -1);
	const int r = value_calc_int(m);
	if (r < 1) {
		raise_error("rnd: Please specify 1 or more.@@ %d", r);
	}
	stack_pop(s->stack_, arg_num);
	const int res = rand() % (r);
	stack_push(s->stack_, create_value(res));
}

void
function_abs(execute_environment_t* e, execute_status_t* s, int arg_num)
{
	if (arg_num != 1) {
		raise_error("abs: Invalid argument.");
	}
	const value_t* m = stack_peek(s->stack_, -1);
	const int r = value_calc_int(m);
	stack_pop(s->stack_, arg_num);
	const int res = (r < 0 ? -r : r);
	stack_push(s->stack_, create_value(res));
}

void
function_powf(execute_environment_t* e, execute_status_t* s, int arg_num)
{
	if (arg_num != 2) {
		raise_error("powf: Invalid argument.");
	}
	const int arg_start = -arg_num;

	const value_t* p1 = stack_peek(s->stack_, arg_start);
	double x = value_calc_double(p1);
	const value_t* p2 = stack_peek(s->stack_, arg_start + 1);
	double y = value_calc_double(p2);

	stack_pop(s->stack_, arg_num);
	stack_push(s->stack_, create_value2(powf((float)x, (float)y)));
}

void
function_peek(execute_environment_t* e, execute_status_t* s, int arg_num)
{
	if (arg_num != 2) {
		raise_error("peek: Invalid argument.");
	}
	const int arg_start = -arg_num;

	// ２つめの引数を取得する
	const value_t* n = stack_peek(s->stack_, arg_start + 1);
	const int i = value_calc_int(n);

	// １つめの引数を取得する
	value_t* m = stack_peek(s->stack_, arg_start);
	value_isolate(m);
	char* p = m->svalue_;
	unsigned char r = p[i];

	stack_pop(s->stack_, arg_num);
	stack_push(s->stack_, create_value(r));
}

// 外部リンケージを持つ人たち、ここから

// 全体
void
initialize_system()
{
#if NHSP_CONFIG_MEMLEAK_DETECTION
	if (s_memory_map_ == nullptr) {
		s_memory_map_ = reinterpret_cast<list_t*>(zmalloc(sizeof(list_t)));
		s_memory_map_->head_ = s_memory_map_->tail_ = nullptr;
	}
#endif
}

void
destroy_system()
{
#if NHSP_CONFIG_MEMLEAK_DETECTION
	if (s_memory_map_ != nullptr) {
		printf("====leaked memories\n");
		auto node = s_memory_map_->head_;
		while (node != nullptr) {
			printf("[%p]\n", node->value_);
			node = node->next_;
		}
		printf("----");
	}
#endif
}

// メモリ
void*
xmalloc(size_t size, const char* block_name)
{
	NHSP_UNUSE(block_name);
	void* res = zmalloc(size);
#if NHSP_CONFIG_MEMLEAK_DETECTION
	{
		zregister_memory(res);
	}
#endif
	return res;
}

void
xfree(void* ptr)
{
#if NHSP_CONFIG_MEMLEAK_DETECTION
	{
		zunregister_memory(ptr);
	}
#endif
	zfree(ptr);
}

void*
xrealloc(void* ptr, size_t size)
{
	void* res = zrealloc(ptr, size);
#if NHSP_CONFIG_MEMLEAK_DETECTION
	{
		if (res != ptr) {
			zunregister_memory(ptr);
			zregister_memory(res);
		}
	}
#endif
	return res;
}

// リスト
list_node_t*
create_list_node()
{
	list_node_t* res =
		(list_node_t*)xmalloc(sizeof(list_node_t), "");
	res->prev_ = res->next_ = NULL;
	res->value_ = NULL;
	return res;
}

void
destroy_list_node(list_node_t* node)
{
	unlink_list_node(node);
	xfree(node);
}

void
link_next(list_node_t* node, list_node_t* list)
{
	assert(node->prev_ == NULL && node->next_ == NULL);
	node->prev_ = list;
	node->next_ = list->next_;
	list->next_ = node;
}

void
link_prev(list_node_t* node, list_node_t* list)
{
	assert(node->prev_ == NULL && node->next_ == NULL);
	node->prev_ = list->prev_;
	node->next_ = list;
	list->prev_ = node;
}

bool
unlink_list_node(list_node_t* node)
{
	if (node->prev_ == NULL && node->next_ == NULL) {
		return false;
	}
	if (node->prev_ != NULL) {
		node->prev_->next_ = node->next_;
	}
	if (node->next_ != NULL) {
		node->next_->prev_ = node->prev_;
	}
	node->prev_ = node->next_ = NULL;
	return true;
}

list_t*
create_list()
{
	list_t* res = (list_t*)xmalloc(sizeof(list_t), "");
	res->head_ = res->tail_ = NULL;
	return res;
}

void
destroy_list(list_t* list)
{
	xfree(list);
}

void
list_prepend(list_t* list, list_node_t* node)
{
	if (list->head_ == NULL) {
		assert(list->tail_ == NULL);
		list->head_ = list->tail_ = node;
		return;
	}
	link_prev(node, list->head_);
	list->head_ = node;
}

void
list_append(list_t* list, list_node_t* node)
{
	if (list->tail_ == NULL) {
		assert(list->head_ == NULL);
		list->head_ = list->tail_ = node;
		return;
	}
	link_next(node, list->tail_);
	list->tail_ = node;
}

void
list_erase(list_t* list, list_node_t* node)
{
	if (list->head_ == node) {
		list->head_ = node->next_;
	}
	if (list->tail_ == node) {
		list->tail_ = node->prev_;
	}
	unlink_list_node(node);
}

list_node_t*
list_find(list_t* list, void* value)
{
	list_node_t* node = list->head_;
	while (node != NULL) {
		if (node->value_ == value) {
			return node;
		}
		node = node->next_;
	}
	return NULL;
}

void
list_free_all(list_t* list)
{
	list_node_t* node = list->head_;
	while (node != NULL) {
		list_node_t* next = node->next_;
		list_erase(list, node);
		xfree(node);
		node = next;
	}
}

// キーワード
int
query_keyword(const char* s)
{
	static const struct
	{
		int tag_;
		const char* word_;
	} table[] = {
		{
			KEYWORD_END, "end",
		},
		{
			KEYWORD_RETURN, "return",
		},
		{
			KEYWORD_GOTO, "goto",
		},
		{
			KEYWORD_GOSUB, "gosub",
		},
		{
			KEYWORD_REPEAT, "repeat",
		},
		{
			KEYWORD_LOOP, "loop",
		},
		{
			KEYWORD_CONTINUE, "continue",
		},
		{
			KEYWORD_BREAK, "break",
		},
		{
			KEYWORD_IF, "if",
		},
		{
			KEYWORD_ELSE, "else",
		},
		{ -1, NULL },
	};
	// 全探索
	for (int i = 0; table[i].tag_ != -1; ++i) {
		if (string_equal_igcase(s, table[i].word_, -1)) {
			return table[i].tag_;
		}
	}
	return -1;
}

// トークナイザ
int
query_token_shadow(const char* ident, size_t len)
{
	static const struct
	{
		int type_;
		const char* word_;
	} shadows[] = {
		{ TOKEN_OP_BAND, "and" },
		{ TOKEN_OP_BOR, "or" },
		{ -1, NULL },
	};
	for (int i = 0; shadows[i].type_ != -1; ++i) {
		if (string_equal_igcase(shadows[i].word_, ident, len)) {
			return shadows[i].type_;
		}
	}
	return -1;
}

void
initialize_tokenize_context(tokenize_context_t* c, const char* script)
{
	c->script_ = script;
	c->cursor_ = 0;
	c->line_ = 0;
	c->line_head_ = script;
}

void
uninitialize_tokenize_context(tokenize_context_t* c)
{
	c->script_ = NULL;
	c->cursor_ = 0;
	c->line_ = 0;
	c->line_head_ = NULL;
}

const bool
is_space(char c)
{
	return (c == ' ' || c == '\t');
}

const bool
is_number(char c)
{
	return (c >= '0' && c <= '9');
}

const bool
is_alpha(char c)
{
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

const bool
is_rest_ident(char c)
{
	return (is_number(c) || is_alpha(c) || c == '_');
}

token_t*
get_token(tokenize_context_t* c)
{
	token_t* res = (token_t*)xmalloc(sizeof(token_t), "");
	res->tag_ = TOKEN_UNKNOWN;
	res->content_ = NULL;
	res->cursor_begin_ = c->cursor_;
	res->cursor_end_ = c->cursor_;
	res->left_space_ = false;
	res->right_space_ = false;
	const char* pp = c->script_ + c->cursor_;
	const char* p = c->script_ + c->cursor_;
	const char* prev_p;// = p;
	int prev_cursor;// = p - c->script_;
restart:
	prev_p = p;
	prev_cursor = p - c->script_;
	res->appear_line_ = c->line_;

	switch (p[0]) {
		// EOF
	case '\0':
		res->tag_ = TOKEN_EOF;
		break;
		// 行終わり
	case '\r':
	case '\f':
		++p;
		goto restart;
	case '\n':
		// この位置はマーキング
		++c->line_;
		++p;
		c->line_head_ = p;
		res->tag_ = TOKEN_EOL;
		break;
		// ステートメント終わり
	case ':':
		++p;
		res->tag_ = TOKEN_EOS;
		break;
		// 微妙な文字
	case '{':
		++p;
		res->tag_ = TOKEN_LBRACE;
		break;
	case '}':
		++p;
		res->tag_ = TOKEN_RBRACE;
		break;
	case '(':
		++p;
		res->tag_ = TOKEN_LPARENTHESIS;
		break;
	case ')':
		++p;
		res->tag_ = TOKEN_RPARENTHESIS;
		break;
	case ',':
		++p;
		res->tag_ = TOKEN_COMMA;
		break;
		// 演算子
	case '|':
		++p;
		res->tag_ = TOKEN_OP_BOR;
		break;
	case '&':
		++p;
		res->tag_ = TOKEN_OP_BAND;
		break;
	case '!':
		++p;
		if (p[0] == '=') {
			++p;
		}
		res->tag_ = TOKEN_OP_NEQ;
		break;
	case '>':
		++p;
		if (p[0] == '=') {
			++p;
			res->tag_ = TOKEN_OP_GTOE;
		}
		else {
			res->tag_ = TOKEN_OP_GT;
		}
		break;
	case '<':
		++p;
		if (p[0] == '=') {
			++p;
			res->tag_ = TOKEN_OP_LTOE;
		}
		else {
			res->tag_ = TOKEN_OP_LT;
		}
		break;
	case '+':
		++p;
		res->tag_ = TOKEN_OP_ADD;
		break;
	case '-':
		++p;
		res->tag_ = TOKEN_OP_SUB;
		break;
	case '*':
		++p;
		res->tag_ = TOKEN_OP_MUL;
		break;
	case '/':
		++p;
		res->tag_ = TOKEN_OP_DIV;
		break;
	case '\\':
		++p;
		res->tag_ = TOKEN_OP_MOD;
		break;
		// 代入
	case '=':
		++p;
		if (p[0] == '=') {
			++p;
			res->tag_ = TOKEN_OP_EQ;
		}
		else {
			res->tag_ = TOKEN_ASSIGN;
		}
		break;
		// 文字列
	case '\"': {
		++p;
		const char* s = p;
		while (p[0] != '\"') {
			if (p[0] == '\0') {
				raise_error("EOF detected in string.@@ %d Row", c->line_);
			}
			if (p[0] == '\\' && p[1] == '\"') {
				++p;
			}
			++p;
		}
		const char* e = p;
		res->content_ = create_token_string(s, e - s);
		res->tag_ = TOKEN_STRING;
		++p;
		break;
	}
		// コメント
	case ';':
		++p;
		while (p[0] != '\n' && p[0] != '\0') {
			++p;
		}
		goto restart;
	default:
		if (is_space(p[0])) {
			// スペース
			++p;
			res->left_space_ = true;
			while (is_space(p[0])) {
				++p;
			}
			goto restart;
		}
		else if (is_number(p[0])) {
			// 数値
			if (p[0] == '0') {
				++p;
			}
			else {
				while (is_number(p[0])) {
					++p;
				}
			}
			if (p[0] == '.') {
				++p;
				while (is_number(p[0])) {
					++p;
				}
				res->tag_ = TOKEN_REAL;
			}
			else {
				res->tag_ = TOKEN_INTEGER;
			}
		}
		else if (is_alpha(p[0])) {
			// 何らかの識別子
			++p;
			while (is_rest_ident(p[0])) {
				++p;
			}
			res->tag_ = TOKEN_IDENTIFIER;
			const int shadow =
				query_token_shadow(c->script_ + prev_cursor, p - prev_p);
			if (shadow != -1) {
				res->tag_ = (token_tag)shadow;
			}
		}
		else {
			// もう読めない
			raise_error("Unknown character[%c]@@ %d Row", p[0], c->line_);
		}
		break;
	}

	if (is_space(p[0])) {
		res->right_space_ = true;
	}
	c->cursor_ += p - pp;
	res->cursor_begin_ = prev_cursor;
	res->cursor_end_ = c->cursor_;
	// contentが埋まってないなら埋める
	if (res->content_ == NULL) {
		const int len = res->cursor_end_ - res->cursor_begin_;
		res->content_ = create_string(len);
		memcpy(res->content_, c->script_ + res->cursor_begin_, len);
		res->content_[len] = '\0';
	}
	return res;
}

void
destroy_token(token_t* t)
{
	if (t->content_ != NULL) {
		destroy_string(t->content_);
		t->content_ = NULL;
	}
	xfree(t);
}

char*
create_token_string(const char* str, size_t len)
{
	char* res = create_string(len);
	size_t w = 0;
	for (size_t i = 0; i < len; ++i, ++w) {
		if (str[i] == '\\' && (i + 1) < len) {
			switch (str[i + 1]) {
			case 't':
				res[w] = '\t';
				break;
			case 'n':
				res[w] = '\n';
				break;
			case '\"':
				res[w] = '\"';
				break;
			default:
				raise_error("Unknown escape sequence@@ %c%c", str[i], str[i + 1]);
				break;
			}
			++i;
		}
		else {
			res[w] = str[i];
		}
	}
	res[w] = '\0';
	return res;
}

// パーサ
parse_context_t*
create_parse_context()
{
	parse_context_t* res =
		(parse_context_t*)xmalloc(sizeof(parse_context_t), "");
	return res;
}

void
destroy_parse_context(parse_context_t* p)
{
	xfree(p);
}

void
initialize_parse_context(parse_context_t* c, tokenize_context_t* t)
{
	c->token_list_ = create_list();
	c->token_current_ = NULL;
	c->tokenize_context_ = t;
}

void
uninitialize_parse_context(parse_context_t* c)
{
	if (c->token_list_ != NULL) {
		list_node_t* iter = c->token_list_->head_;
		while (iter != NULL) {
			list_node_t* next = iter->next_;
			token_t* token = (token_t*)iter->value_;
			destroy_token(token);
			destroy_list_node(iter);
			iter = next;
		}
		destroy_list(c->token_list_);
	}
	c->token_list_ = NULL;
	c->token_current_ = NULL;
}

token_t*
read_token(parse_context_t* c)
{
	if (c->token_current_ == NULL) {
		list_node_t* node = create_list_node();
		node->value_ = get_token(c->tokenize_context_);
		list_append(c->token_list_, node);
		c->token_current_ = node;
	}
	assert(c->token_current_ != NULL);
	const list_node_t* res = c->token_current_;
	c->token_current_ = c->token_current_->next_;
	return (token_t*)res->value_;
}

void
unread_token(parse_context_t* c, size_t num)
{
	if (num <= 0) {
		return;
	}
	if (c->token_current_ == NULL) {
		c->token_current_ = c->token_list_->tail_;
		--num;
	}
	while (num-- > 0) {
		assert(c->token_current_ != NULL);
		c->token_current_ = c->token_current_->prev_;
	}
}

token_t*
prev_token(parse_context_t* c, size_t num)
{
	list_node_t* current = c->token_current_;
	if (current == NULL) {
		current = c->token_list_->tail_;
	}
	while (num-- > 0) {
		assert(current != NULL);
		current = current->prev_;
	}
	return (token_t*)current->value_;
}

ast_node_t*
create_ast_node(node_tag tag, ast_node_t* left, ast_node_t* right)
{
	ast_node_t* res = (ast_node_t*)xmalloc(sizeof(ast_node_t), "");
	res->tag_ = tag;
	res->token_ = NULL;
	res->left_ = left;
	res->right_ = right;
	res->flag_ = 0;
	res->ext_ = NULL;
	return res;
}

ast_node_t*
create_ast_node2(node_tag tag, token_t* token, ast_node_t* left)
{
	ast_node_t* res = (ast_node_t*)xmalloc(sizeof(ast_node_t), "");
	res->tag_ = tag;
	res->token_ = token;
	res->left_ = left;
	res->right_ = NULL;
	res->flag_ = 0;
	res->ext_ = NULL;
	return res;
}

void
destroy_ast_node(ast_node_t* node)
{
	assert(node != NULL);
	if (node->left_ != NULL) {
		destroy_ast_node(node->left_);
	}
	if (node->right_ != NULL) {
		destroy_ast_node(node->right_);
	}
	xfree(node);
}

bool
is_eos_like_token(token_tag tag)
{
	return (tag == TOKEN_EOF || tag == TOKEN_EOL || tag == TOKEN_EOS ||
		tag == TOKEN_RBRACE);
}

list_t*
parse_script(parse_context_t* c)
{
	list_t* res = create_list();
	for (;;) {
		ast_node_t* statement = parse_statement(c);
		if (statement == NULL) {
			break;
		}
		list_node_t* node = create_list_node();
		node->value_ = statement;
		list_append(res, node);
	}
	{
		const token_t* token = read_token(c);
		if (token->tag_ != TOKEN_EOF) {
			raise_error("Script couldn't be parsed successfully.@@ %d Row", token->appear_line_);
		}
	}
	return res;
}

void
destroy_ast(list_t* ast)
{
	list_node_t* node = ast->head_;
	while (node != NULL) {
		ast_node_t* ast_node = (ast_node_t*)node->value_;
		destroy_ast_node(ast_node);
		node = node->next_;
	}
	list_free_all(ast);
	destroy_list(ast);
}

ast_node_t*
parse_statement(parse_context_t* c)
{
	// 何もない？
	{
		const token_t* token = read_token(c);
		if (token->tag_ == TOKEN_EOF) {
			return NULL;
		}
		if (is_eos_like_token(token->tag_)) {
			return create_ast_node(NODE_EMPTY, NULL, NULL);
		}
		unread_token(c, 1);
	}
	ast_node_t* statement = NULL;
	// ラベルを試す
	if (statement == NULL) {
		statement = parse_label_safe(c);
	}
	// 制御構文を試す
	if (statement == NULL) {
		statement = parse_control_safe(c);
	}
	// コマンドを試す
	if (statement == NULL) {
		statement = parse_command_safe(c);
	}
	// それ以外は全て代入
	if (statement == NULL) {
		statement = parse_assign_safe(c);
	}
	// ここまで来て何もないなら、パース不能
	if (statement == NULL) {
		const token_t* token = read_token(c);
		raise_error("Statement cannot be parsed.@@ %d Row", token->appear_line_);
	}
	// 最後の改行チェック
	const token_t* token = read_token(c);
	if (!is_eos_like_token(token->tag_)) {
		raise_error("Statement couldn't be parsed successfully.@@ %d Row", token->appear_line_);
	}
	return statement;
}

ast_node_t*
parse_label_safe(parse_context_t* c)
{
	const token_t* token = read_token(c);
	if (token->tag_ != TOKEN_OP_MUL) {
		unread_token(c, 1);
		return NULL;
	}
	token_t* ident = read_token(c);
	if (ident->tag_ != TOKEN_IDENTIFIER) {
		unread_token(c, 2);
		return NULL;
	}
	return create_ast_node2(NODE_LABEL, ident, NULL);
}

const bool
is_else_token(const token_t* n)
{
	if (n->tag_ == TOKEN_IDENTIFIER &&
		query_keyword(n->content_) == KEYWORD_ELSE) {
		return true;
	}
	return false;
}

ast_node_t*
parse_control_safe(parse_context_t* c)
{
	token_t* ident = read_token(c);
	if (ident->tag_ != TOKEN_IDENTIFIER) {
		unread_token(c, 1);
		return NULL;
	}
	const int keyword = query_keyword(ident->content_);
	if (keyword < 0) {
		unread_token(c, 1);
		return NULL;
	}

	switch (keyword) {
	case KEYWORD_END:
		return create_ast_node(NODE_END, NULL, NULL);
	case KEYWORD_RETURN: {
		const token_t* next = read_token(c);
		unread_token(c, 1);
		ast_node_t* expr = NULL;
		if (!is_eos_like_token(next->tag_)) {
			expr = parse_expression(c);
		}
		return create_ast_node(NODE_RETURN, expr, NULL);
	}
	case KEYWORD_GOTO:
	case KEYWORD_GOSUB: {
		ast_node_t* label = parse_label_safe(c);
		if (label == NULL) {
			raise_error("Labels must be specified for goto or gosub.@@ %d Row", ident->appear_line_);
		}
		node_tag tmp = keyword == KEYWORD_GOTO ? NODE_GOTO : NODE_GOSUB;
		return create_ast_node(tmp, label, NULL);
	}
	case KEYWORD_REPEAT: {
		const token_t* next = read_token(c);
		unread_token(c, 1);
		ast_node_t* expr = NULL;
		if (!is_eos_like_token(next->tag_)) {
			expr = parse_expression(c);
		}
		return create_ast_node(NODE_REPEAT, expr, NULL);
	}
	case KEYWORD_LOOP:
		return create_ast_node(NODE_LOOP, NULL, NULL);
	case KEYWORD_CONTINUE:
		return create_ast_node(NODE_CONTINUE, NULL, NULL);
	case KEYWORD_BREAK:
		return create_ast_node(NODE_BREAK, NULL, NULL);
	case KEYWORD_IF: {
		ast_node_t* expr = parse_expression(c);
		const token_t* next = read_token(c);
		bool repair_token = false;
		ast_node_t* true_statements = create_ast_node(NODE_BLOCK_STATEMENTS, NULL, NULL);
		ast_node_t* false_statements = NULL;
		if (next->tag_ == TOKEN_LBRACE) {
			for (;;) {
				const token_t* pp = prev_token(c, 0);
				if (pp->tag_ == TOKEN_RBRACE) {
					// RBRACEは食われてた
					break;
				}
				ast_node_t* statement = parse_statement(c);
				if (statement == NULL) {
					raise_error("if contains statements that cannot be parsed.@@ %d to %d Row", pp->appear_line_, ident->appear_line_);
				}
				true_statements =
					create_ast_node(NODE_BLOCK_STATEMENTS, true_statements, statement);
			}
		}
		else {
			unread_token(c, 1);
			{
				const token_t* nn = read_token(c);
				if (nn->tag_ != TOKEN_EOS) {
					raise_error("After if conditional, only { or : @@ %d Row", nn->appear_line_);
				}
			}
			for (;;) {
				const token_t* pp = prev_token(c, 0);
				if (pp->tag_ == TOKEN_EOL) {
					// EOLは食われてた
					repair_token = true;
					break;
				}
				const token_t* nn = read_token(c);
				unread_token(c, 1);
				if (is_else_token(nn)) {
					break;
				}
				ast_node_t* statement = parse_statement(c);
				if (statement == NULL) {
					raise_error("if contains statements that cannot be parsed.@@ %d to %d Row", nn->appear_line_, ident->appear_line_);
				}
				true_statements =
					create_ast_node(NODE_BLOCK_STATEMENTS, true_statements, statement);
			}
		}
		// elseはあるか？
		const token_t* nn = read_token(c);
		if (is_else_token(nn)) {
			repair_token = false;
			false_statements = create_ast_node(NODE_BLOCK_STATEMENTS, NULL, NULL);
			const token_t* nextf = read_token(c);
			if (nextf->tag_ == TOKEN_LBRACE) {
				for (;;) {
					const token_t* ppf = prev_token(c, 0);
					if (ppf->tag_ == TOKEN_RBRACE) {
						// RBRACEは食われてた
						break;
					}
					ast_node_t* statement = parse_statement(c);
					if (statement == NULL) {
						raise_error("else contains statements that cannot be parsed.@@ %d to %d Row", nn->appear_line_, ident->appear_line_);
					}
					false_statements = create_ast_node(NODE_BLOCK_STATEMENTS, false_statements, statement);
				}
			}
			else {
				unread_token(c, 1);
				{
					const token_t* nnf = read_token(c);
					if (nnf->tag_ != TOKEN_EOS) {
						raise_error("After else, only { or : @@ %d Row", nnf->appear_line_);
					}
				}
				for (;;) {
					const token_t* pp = prev_token(c, 0);
					if (pp->tag_ == TOKEN_EOL) {
						// EOLは食われてた
						repair_token = true;
						break;
					}
					const token_t* nnf = read_token(c);
					unread_token(c, 1);
					if (is_else_token(nnf)) {
						break;
					}
					ast_node_t* statement = parse_statement(c);
					if (statement == NULL) {
						raise_error( "else contains statements that cannot be parsed.@@ %d to %d Row", nnf->appear_line_, ident->appear_line_ );
					}
					false_statements = create_ast_node(NODE_BLOCK_STATEMENTS, false_statements, statement);
				}
			}
		}
		else {
			unread_token(c, 1);
		}
		// EOLを食いすぎてしまった場合用
		if (repair_token) {
			unread_token(c, 1);
		}
		ast_node_t* dispatcher =
			create_ast_node(NODE_IF_DISPATCHER, true_statements, false_statements);
		return create_ast_node(NODE_IF, expr, dispatcher);
	}
	case KEYWORD_ELSE:
		raise_error("Detected an unprocessed else.@@ %d Row", ident->appear_line_);
		break;
	}
	unread_token(c, 1);
	return NULL;
}

ast_node_t*
parse_command_safe(parse_context_t* c)
{
	token_t* ident = read_token(c);
	if (ident->tag_ != TOKEN_IDENTIFIER) {
		unread_token(c, 1);
		return NULL;
	}
	const token_t* next = read_token(c);
	bool is_not_command = false;
	if (next->tag_ == TOKEN_ASSIGN) {
		is_not_command = true;
	}
	// "("チェック
	if (!ident->right_space_ && next->tag_ == TOKEN_LPARENTHESIS) {
		is_not_command = true;
	}
	if (is_not_command) {
		unread_token(c, 2);
		return NULL;
	}
	// あるなら引数の解析
	ast_node_t* args = NULL;
	if (!is_eos_like_token(next->tag_)) {
		unread_token(c, 1);
		args = parse_arguments(c);
	}
	else {
		unread_token(c, 1);
	}
	ast_node_t* command = create_ast_node2(NODE_COMMAND, ident, args);
	return command;
}

ast_node_t*
parse_arguments(parse_context_t* c)
{
	ast_node_t* arg = parse_expression(c);
	ast_node_t* res = create_ast_node(NODE_ARGUMENTS, arg, NULL);
	ast_node_t* args = res;
	for (;;) {
		const token_t* token = read_token(c);
		if (token->tag_ != TOKEN_COMMA) {
			unread_token(c, 1);
			break;
		}
		ast_node_t* next = parse_expression(c);
		args->right_ = create_ast_node(NODE_ARGUMENTS, next, NULL);
		args = args->right_;
	}
	return res;
}

ast_node_t*
parse_assign_safe(parse_context_t* c)
{
	ast_node_t* variable = parse_variable_safe(c);
	if (variable == NULL) {
		return NULL;
	}
	const token_t* next = read_token(c);
	if (next->tag_ != TOKEN_ASSIGN) {
		raise_error("Value substitution: = is required.@@ %d Row", next->appear_line_);
	}
	ast_node_t* expr = parse_expression(c);
	ast_node_t* assign = create_ast_node(NODE_ASSIGN, variable, expr);
	return assign;
}

ast_node_t*
parse_variable_safe(parse_context_t* c)
{
	token_t* ident = read_token(c);
	if (ident->tag_ != TOKEN_IDENTIFIER) {
		unread_token(c, 1);
		return NULL;
	}
	const token_t* next = read_token(c);
	if (next->tag_ != TOKEN_LPARENTHESIS) {
		unread_token(c, 1);
		return create_ast_node2(NODE_VARIABLE, ident, NULL);
	}
	ast_node_t* idx = parse_expression(c);
	const token_t* nn = read_token(c);
	if (nn->tag_ != TOKEN_RPARENTHESIS) {
		// 多そうなので個別対処
		if (nn->tag_ == TOKEN_COMMA) {
			raise_error("Array variable is one dimension only.@@ %d Row", nn->appear_line_);
		}
		raise_error("Array variable: Parentheses are not closed.@@ %d Row", nn->appear_line_);
	}
	return create_ast_node2(NODE_VARIABLE, ident, idx);
}

ast_node_t*
parse_expression(parse_context_t* c)
{
	// ただの関数転送
	return parse_borand(c);
}

ast_node_t*
parse_borand(parse_context_t* c)
{
	ast_node_t* node = parse_eqneq(c);
	for (;;) {
		bool is_continue = true;
		const token_t* token = read_token(c);
		switch (token->tag_) {
		case TOKEN_OP_BOR:
		case TOKEN_OP_BAND: {
			ast_node_t* r = parse_eqneq(c);
			if (r == NULL) {
				raise_error("|,& Operator analysis failed.@@ %d Row", token->appear_line_);
			}
			switch (token->tag_) {
			case TOKEN_OP_BOR:
				node = create_ast_node(NODE_BOR, node, r);
				break;
			case TOKEN_OP_BAND:
				node = create_ast_node(NODE_BAND, node, r);
				break;
			default:
				assert(false);
				break;
			}
			break;
		}
		default:
			is_continue = false;
			break;
		}
		if (!is_continue) {
			unread_token(c, 1);
			break;
		}
	}
	return node;
}

ast_node_t*
parse_eqneq(parse_context_t* c)
{
	ast_node_t* node = parse_gtlt(c);
	for (;;) {
		bool is_continue = true;
		const token_t* token = read_token(c);
		switch (token->tag_) {
		case TOKEN_OP_EQ:
		case TOKEN_OP_NEQ:
		case TOKEN_ASSIGN: {
			ast_node_t* r = parse_gtlt(c);
			if (r == NULL) {
				raise_error("==,!= Operator analysis failed.@@ %d Row", token->appear_line_);
			}
			switch (token->tag_) {
			case TOKEN_OP_EQ:
			case TOKEN_ASSIGN:
				node = create_ast_node(NODE_EQ, node, r);
				break;
			case TOKEN_OP_NEQ:
				node = create_ast_node(NODE_NEQ, node, r);
				break;
			default:
				assert(false);
				break;
			}
			break;
		}
		default:
			is_continue = false;
			break;
		}
		if (!is_continue) {
			unread_token(c, 1);
			break;
		}
	}
	return node;
}

ast_node_t*
parse_gtlt(parse_context_t* c)
{
	ast_node_t* node = parse_addsub(c);
	for (;;) {
		bool is_continue = true;
		const token_t* token = read_token(c);
		switch (token->tag_) {
		case TOKEN_OP_GT:
		case TOKEN_OP_GTOE:
		case TOKEN_OP_LT:
		case TOKEN_OP_LTOE: {
			ast_node_t* r = parse_addsub(c);
			if (r == NULL) {
				raise_error(">,>=,<,<= Operator analysis failed.@@ %d Row", token->appear_line_);
			}
			switch (token->tag_) {
			case TOKEN_OP_GT:
				node = create_ast_node(NODE_GT, node, r);
				break;
			case TOKEN_OP_GTOE:
				node = create_ast_node(NODE_GTOE, node, r);
				break;
			case TOKEN_OP_LT:
				node = create_ast_node(NODE_LT, node, r);
				break;
			case TOKEN_OP_LTOE:
				node = create_ast_node(NODE_LTOE, node, r);
				break;
			default:
				assert(false);
				break;
			}
			break;
		}
		default:
			is_continue = false;
			break;
		}
		if (!is_continue) {
			unread_token(c, 1);
			break;
		}
	}
	return node;
}

ast_node_t*
parse_addsub(parse_context_t* c)
{
	ast_node_t* node = parse_muldivmod(c);
	for (;;) {
		bool is_continue = true;
		const token_t* token = read_token(c);
		switch (token->tag_) {
		case TOKEN_OP_ADD:
		case TOKEN_OP_SUB: {
			ast_node_t* r = parse_muldivmod(c);
			if (r == NULL) {
				raise_error("+- Operator analysis failed.@@ %d Row", token->appear_line_);
			}
			switch (token->tag_) {
			case TOKEN_OP_ADD:
				node = create_ast_node(NODE_ADD, node, r);
				break;
			case TOKEN_OP_SUB:
				node = create_ast_node(NODE_SUB, node, r);
				break;
			default:
				assert(false);
				break;
			}
			break;
		}
		default:
			is_continue = false;
			break;
		}
		if (!is_continue) {
			unread_token(c, 1);
			break;
		}
	}
	return node;
}

ast_node_t*
parse_muldivmod(parse_context_t* c)
{
	ast_node_t* node = parse_term(c);
	for (;;) {
		bool is_continue = true;
		const token_t* token = read_token(c);
		switch (token->tag_) {
		case TOKEN_OP_MUL:
		case TOKEN_OP_DIV:
		case TOKEN_OP_MOD: {
			ast_node_t* r = parse_term(c);
			if (r == NULL) {
				raise_error("*/\\ Operator analysis failed.@@ %d Row", token->appear_line_);
			}
			switch (token->tag_) {
			case TOKEN_OP_MUL:
				node = create_ast_node(NODE_MUL, node, r);
				break;
			case TOKEN_OP_DIV:
				node = create_ast_node(NODE_DIV, node, r);
				break;
			case TOKEN_OP_MOD:
				node = create_ast_node(NODE_MOD, node, r);
				break;
			default:
				assert(false);
				break;
			}
			break;
		}
		default:
			is_continue = false;
			break;
		}
		if (!is_continue) {
			unread_token(c, 1);
			break;
		}
	}
	return node;
}

ast_node_t*
parse_term(parse_context_t* c)
{
	const token_t* token = read_token(c);
	switch (token->tag_) {
	case TOKEN_OP_SUB:
		return create_ast_node(NODE_UNARY_MINUS, parse_primitive(c), NULL);
	default:
		break;
	}
	unread_token(c, 1);
	return parse_primitive(c);
}

ast_node_t*
parse_primitive(parse_context_t* c)
{
	token_t* token = read_token(c);
	switch (token->tag_) {
	case TOKEN_LPARENTHESIS: {
		ast_node_t* node = parse_expression(c);
		const token_t* next = read_token(c);
		if (next->tag_ != TOKEN_RPARENTHESIS) {
			raise_error("Parentheses are not closed.@@ %d Row", token->appear_line_);
		}
		return create_ast_node(NODE_EXPRESSION, node, NULL);
	}
	case TOKEN_INTEGER:
	case TOKEN_REAL:
	case TOKEN_STRING:
		return create_ast_node2(NODE_PRIMITIVE_VALUE, token, NULL);
	case TOKEN_OP_MUL: {
		unread_token(c, 1);
		ast_node_t* label = parse_label_safe(c);
		if (label == NULL) {
			raise_error("Label cannot be parsed.@@ %d Row", token->appear_line_);
		}
		raise_error("Labels cannot be used for expressions.@@ %d Row", token->appear_line_);
		return label;
	}
	case TOKEN_IDENTIFIER: {
		unread_token(c, 1);
		ast_node_t* expr = parse_identifier_expression(c);
		if (expr == NULL) {
			raise_error("Function or Variable cannot be parsed.@@ %d Row", token->appear_line_);
		}
		return expr;
	}
	default:
		break;
	}
	raise_error("Primitive values cannot be acquired.@@ %d Row[%s]", token->appear_line_, token->content_);
	return NULL;
}

ast_node_t*
parse_identifier_expression(parse_context_t* c)
{
	token_t* ident = read_token(c);
	if (ident->tag_ != TOKEN_IDENTIFIER) {
		unread_token(c, 1);
		return NULL;
	}
	const token_t* next = read_token(c);
	if (next->tag_ != TOKEN_LPARENTHESIS) {
		unread_token(c, 1);
		return create_ast_node2(NODE_IDENTIFIER_EXPR, ident, NULL);
	}
	// 引数なしの即閉じ？
	{
		const token_t* nn = read_token(c);
		if (nn->tag_ == TOKEN_RPARENTHESIS) {
			return create_ast_node2(NODE_IDENTIFIER_EXPR, ident, create_ast_node(NODE_ARGUMENTS, NULL, NULL));
		}
		unread_token(c, 1);
	}
	// 引数あり
	ast_node_t* idx = parse_arguments(c);
	const token_t* nn = read_token(c);
	if (nn->tag_ != TOKEN_RPARENTHESIS) {
		raise_error("Function or Array variable: Parentheses are not closed.@@ %d Row", nn->appear_line_);
	}
	return create_ast_node2(NODE_IDENTIFIER_EXPR, ident, idx);
}

// 変数
variable_t*
create_variable(const char* name)
{

	variable_t* res = (variable_t*)xmalloc(sizeof(variable_t), "");
	res->name_ = create_string3(name);
	res->type_ = VALUE_NONE;
	res->granule_size_ = 0;
	res->length_ = 0;
	res->data_ = NULL;
	prepare_variable(res, VALUE_INT, 64, 16);
	return res;
}

void
destroy_variable(variable_t* v)
{
	xfree(v->name_);
	xfree(v->data_);
	xfree(v);
}

void
prepare_variable(variable_t* v, value_tag type, int granule_size, int length)
{
	if (v->data_ != NULL) {
		xfree(v->data_);
		v->data_ = NULL;
	}
	v->type_ = type;
	v->granule_size_ = granule_size;
	v->length_ = length;
	size_t areasize = 0;
	switch (type) {
	case VALUE_INT:
		areasize = sizeof(int) * v->length_;
		break;
	case VALUE_DOUBLE:
		areasize = sizeof(double) * v->length_;
		break;
	case VALUE_STRING:
		areasize = sizeof(char) * v->granule_size_ * v->length_;
		break;
	default:
		assert(false);
		break;
	}
	assert(areasize > 0);
	v->data_ = xmalloc(areasize, "");
	memset(v->data_, 0, areasize);
}

list_t*
create_variable_table()
{
	return create_list();
}

void
destroy_variable_table(list_t* table)
{
	list_node_t* node = table->head_;
	while (node != NULL) {
		variable_t* var = (variable_t*)node->value_;
		destroy_variable(var);
		node = node->next_;
	}
	list_free_all(table);
	destroy_list(table);
}

variable_t*
search_variable(list_t* table, const char* name)
{
	list_node_t* node = table->head_;
	while (node != NULL) {
		variable_t* var = (variable_t*)node->value_;
		if (name == NULL) {
			if (string_equal_igcase(var->name_, "", -1)) {
				return var;
			}
		}
		else {
			if (string_equal_igcase(var->name_, name, -1)) {
				return var;
			}
		}
		node = node->next_;
	}
	return NULL;
}

void
variable_set(list_t* table, const value_t* v, const char* name, int idx)
{

	variable_t* var = search_variable(table, name);

	if (var == NULL) {
		var = create_variable(name);
		list_node_t* node = create_list_node();
		node->value_ = var;
		list_append(table, node);
	}
	assert(var != NULL);
	if (var->type_ != v->type_) {
		if (idx > 0) {
			raise_error("Variable type is different.@@ %s(%d)", name, idx);
		}
		prepare_variable(var, v->type_, 64, 16);
	}
	bool init_required = false;
	int granule_size = 0;
	if (v->type_ == VALUE_STRING) {
		granule_size = strlen(v->svalue_) + 1;
	}
	if (var->granule_size_ < granule_size) {
		init_required = true;
	}
	int len = var->length_;
	if (idx < 0 || len <= idx) {
		raise_error("Invalid value.@@ %s(%d)", name, idx);
	}
	if (init_required) {
		prepare_variable(var, v->type_, granule_size, len);
	}

	assert(var->type_ == v->type_);
	void* data_ptr = variable_data_ptr(var, idx);

	switch (var->type_) {
	case VALUE_INT: {

		int* tmp = (int*)data_ptr;
		tmp[0] = v->ivalue_;
		break;
	}
	case VALUE_DOUBLE: {
		double* tmp = (double*)data_ptr;
		tmp[0] = v->dvalue_;
		break;
	}
	case VALUE_STRING: {
		char* tmp = (char*)data_ptr;
		strcpy(tmp, v->svalue_);
		break;
	}
	default:
		assert(false);
		break;
	}
}

int count;

void*
variable_data_ptr(const variable_t* v, int idx)
{
	//count++;
	if (v == NULL) {
		//return NULL;
		if (idx < 0 || idx >= 0) {
			raise_error("Variable is out of range.@@ %s(%d)", "???", idx);
		}
	}
	else {
		if (idx < 0 || idx >= v->length_) {
			raise_error("Variable is out of range.@@ %s(%d)", v->name_, idx);
		}
	}
	switch (v->type_) {
	case VALUE_INT: {
		return (int*)v->data_ + idx;
	}
	case VALUE_DOUBLE: {
		return (double*)v->data_ + idx;
	}
	case VALUE_STRING: {
		return (char*)v->data_ + v->granule_size_ * idx;
	}
	default:
		assert(false);
		break;
	}
	return NULL;
}

int
variable_calc_int(const variable_t* r, int idx)
{
	const void* data_ptr = variable_data_ptr(r, idx);
	switch (r->type_) {
	case VALUE_INT:
		return *(int*)data_ptr;
	case VALUE_DOUBLE:
		return (int)(*(double*)data_ptr);
	case VALUE_STRING:
		return atoi((const char*)data_ptr);
	default:
		assert(false);
		break;
	}
	return 0;
}

double
variable_calc_double(const variable_t* r, int idx)
{
	const void* data_ptr = variable_data_ptr(r, idx);
	switch (r->type_) {
	case VALUE_INT:
		return (double)(*(int*)data_ptr);
	case VALUE_DOUBLE:
		return *(double*)data_ptr;
	case VALUE_STRING:
		return atof((const char*)data_ptr);
	default:
		assert(false);
		break;
	}
	return 0.0;
}

char*
variable_calc_string(const variable_t* r, int idx)
{
	const void* data_ptr = variable_data_ptr(r, idx);
	switch (r->type_) {
	case VALUE_INT:
		return create_string_from(*(int*)data_ptr);
	case VALUE_DOUBLE:
		return create_string_from((int)*(double*)data_ptr);
	case VALUE_STRING:
		return create_string3((const char*)data_ptr);
	default:
		assert(false);
		break;
	}
	return create_string3("");
}

// 値
static value_t*
alloc_value()
{
	return (value_t*)xmalloc(sizeof(value_t), "");
}

value_t*
create_value(int v)
{
	value_t* res = alloc_value();
	res->type_ = VALUE_INT;
	res->ivalue_ = v;
	return res;
}

value_t*
create_value2(double v)
{
	value_t* res = alloc_value();
	res->type_ = VALUE_DOUBLE;
	res->dvalue_ = v;
	return res;
}

value_t*
create_value3(const char* v)
{
	value_t* res = alloc_value();
	res->type_ = VALUE_STRING;
	res->svalue_ = create_string3(v);
	return res;
}

value_t*
create_value4(variable_t* v, int idx)
{
	value_t* res = alloc_value();
	res->type_ = VALUE_VARIABLE;
	res->variable_ = v;
	res->index_ = idx;
	return res;
}

value_t*
create_value5(const value_t* v)
{
	value_t* res = alloc_value();
	res->type_ = v->type_;
	switch (v->type_) {
	case VALUE_INT:
		res->ivalue_ = v->ivalue_;
		break;
	case VALUE_DOUBLE:
		res->dvalue_ = v->dvalue_;
		break;
	case VALUE_STRING:
		res->svalue_ = create_string3(v->svalue_);
		break;
	case VALUE_VARIABLE:
		res->variable_ = v->variable_;
		res->index_ = v->index_;
		break;
	default:
		raise_error("Copying an empty value and failed.@@ ptr=%p", &v);
	}
	return res;
}

value_t*
create_value_move(char* v)
{
	value_t* res = alloc_value();
	res->type_ = VALUE_STRING;
	res->svalue_ = v;
	return res;
}

void
destroy_value(value_t* t)
{
	clear_value(t);
	t->type_ = VALUE_NONE;
	t->value_ = 0;
	xfree(t);
}

void
value_set(value_t* v, int i)
{
	clear_value(v);
	v->type_ = VALUE_INT;
	v->ivalue_ = i;
}

void
value_set2(value_t* v, double d)
{
	clear_value(v);
	v->type_ = VALUE_DOUBLE;
	v->dvalue_ = d;
}

void
value_set3(value_t* v, const char* s)
{
	clear_value(v);
	v->type_ = VALUE_STRING;
	v->svalue_ = create_string3(s);
}

void
value_move(value_t* v, char* s)
{
	clear_value(v);
	v->type_ = VALUE_STRING;
	v->svalue_ = s;
}

void
value_move2(value_t* to, value_t* from)
{
	clear_value(to);
	to->type_ = from->type_;
	to->value_ = from->value_;
	from->type_ = VALUE_NONE;
	from->value_ = 0;
}

value_tag
value_get_primitive_tag(const value_t* r)
{
	if (r->type_ == VALUE_VARIABLE) {
		return r->variable_->type_;
	}
	return r->type_;
}

bool
value_calc_boolean(const value_t* r)
{
	switch (value_get_primitive_tag(r)) {
	case VALUE_INT:
		return value_calc_int(r) != 0;
	case VALUE_DOUBLE:
		return value_calc_double(r) != 0.0;
	case VALUE_STRING:
		return value_calc_int(r) != 0;
	default:
		assert(false);
		break;
	}
	return false;
}

int
value_calc_int(const value_t* r)
{
	switch (r->type_) {
	case VALUE_INT:
		return r->ivalue_;
	case VALUE_DOUBLE:
		return (int)r->dvalue_;
	case VALUE_STRING:
		return atoi(r->svalue_);
	case VALUE_VARIABLE:
		return variable_calc_int(r->variable_, r->index_);
		break;
	default:
		assert(false);
		break;
	}
	return 0;
}

double
value_calc_double(const value_t* r)
{
	switch (r->type_) {
	case VALUE_INT:
		return (double)r->ivalue_;
	case VALUE_DOUBLE:
		return r->dvalue_;
	case VALUE_STRING:
		return atof(r->svalue_);
	case VALUE_VARIABLE:
		return variable_calc_double(r->variable_, r->index_);
		break;
	default:
		assert(false);
		break;
	}
	return 0.0;
}

char*
value_calc_string(const value_t* r)
{
	char* s = NULL;
	switch (r->type_) {
	case VALUE_INT:
		s = create_string_from(r->ivalue_);
		break;
	case VALUE_DOUBLE:
		s = create_string_from2(r->dvalue_);
		break;
	case VALUE_STRING:
		s = create_string3(r->svalue_);
		break;
	case VALUE_VARIABLE:
		s = variable_calc_string(r->variable_, r->index_);
		break;
	default:
		assert(false);
		break;
	}
	return s;
}

value_t*
value_convert_type(value_tag to, const value_t* r)
{
	if (to == r->type_) {
		return create_value5(r);
	}
	switch (to) {
	case VALUE_INT:
		return create_value(value_calc_int(r));
	case VALUE_DOUBLE:
		return create_value2(value_calc_double(r));
	case VALUE_STRING: {
		char* s = value_calc_string(r);
		value_t* res = create_value_move(s);
		return res;
	}
	default:
		assert(false);
		break;
	}
	return NULL;
}

void
value_isolate(value_t* v)
{
	if (v->type_ != VALUE_VARIABLE) {
		return;
	}
	if (v->variable_ != NULL) {
		switch (v->variable_->type_) {
		case VALUE_INT:
			value_set(v, variable_calc_int(v->variable_, v->index_));
			break;
		case VALUE_DOUBLE:
			value_set2(v, variable_calc_double(v->variable_, v->index_));
			break;
		case VALUE_STRING:
			value_move(v, variable_calc_string(v->variable_, v->index_));
			break;
		default:
			assert(false);
			break;
		}
	}
}

void
value_bor(value_t* v, const value_t* r)
{
	value_t* t = value_convert_type(v->type_, r);
	switch (v->type_) {
	case VALUE_INT:
		value_set(v, v->ivalue_ | t->ivalue_);
		break;
	case VALUE_DOUBLE: {
		raise_error("| operator for the floating point is not defined.");
		break;
	}
	case VALUE_STRING: {
		raise_error("| operator for the string is not defined.");
		break;
	}
	default:
		assert(false);
		break;
	}
	destroy_value(t);
}

void
value_band(value_t* v, const value_t* r)
{
	value_t* t = value_convert_type(v->type_, r);
	switch (v->type_) {
	case VALUE_INT:
		value_set(v, v->ivalue_ & t->ivalue_);
		break;
	case VALUE_DOUBLE: {
		raise_error("& operator for the floating point is not defined.");
		break;
	}
	case VALUE_STRING: {
		raise_error("& operator for the string is not defined.");
		break;
	}
	default:
		assert(false);
		break;
	}
	destroy_value(t);
}

void
value_eq(value_t* v, const value_t* r)
{
	value_t* t = value_convert_type(v->type_, r);
	switch (v->type_) {
	case VALUE_INT:
		value_set(v, v->ivalue_ == t->ivalue_ ? 1 : 0);
		break;
	case VALUE_DOUBLE:
		value_set(v, v->dvalue_ == t->dvalue_ ? 1 : 0);
		break;
	case VALUE_STRING:
		value_set(v, strcmp(v->svalue_, t->svalue_) == 0 ? 1 : 0);
		break;
	default:
		assert(false);
		break;
	}
	destroy_value(t);
}

void
value_neq(value_t* v, const value_t* r)
{
	value_t* t = value_convert_type(v->type_, r);
	switch (v->type_) {
	case VALUE_INT:
		value_set(v, v->ivalue_ != t->ivalue_ ? 1 : 0);
		break;
	case VALUE_DOUBLE:
		value_set(v, v->dvalue_ != t->dvalue_ ? 1 : 0);
		break;
	case VALUE_STRING:
		value_set(v, strcmp(v->svalue_, t->svalue_) != 0 ? 1 : 0);
		break;
	default:
		assert(false);
		break;
	}
	destroy_value(t);
}

void
value_gt(value_t* v, const value_t* r)
{
	value_t* t = value_convert_type(v->type_, r);
	switch (v->type_) {
	case VALUE_INT:
		value_set(v, v->ivalue_ > t->ivalue_ ? 1 : 0);
		break;
	case VALUE_DOUBLE:
		value_set(v, v->dvalue_ > t->dvalue_ ? 1 : 0);
		break;
	case VALUE_STRING: {
		raise_error("> operator for the string is not defined.");
		break;
	}
	default:
		assert(false);
		break;
	}
	destroy_value(t);
}

void
value_gtoe(value_t* v, const value_t* r)
{
	value_t* t = value_convert_type(v->type_, r);
	switch (v->type_) {
	case VALUE_INT:
		value_set(v, v->ivalue_ >= t->ivalue_ ? 1 : 0);
		break;
	case VALUE_DOUBLE:
		value_set(v, v->dvalue_ >= t->dvalue_ ? 1 : 0);
		break;
	case VALUE_STRING: {
		raise_error(">= operator for the string is not defined.");
		break;
	}
	default:
		assert(false);
		break;
	}
	destroy_value(t);
}

void
value_lt(value_t* v, const value_t* r)
{
	value_t* t = value_convert_type(v->type_, r);
	switch (v->type_) {
	case VALUE_INT:
		value_set(v, v->ivalue_ < t->ivalue_ ? 1 : 0);
		break;
	case VALUE_DOUBLE:
		value_set(v, v->dvalue_ < t->dvalue_ ? 1 : 0);
		break;
	case VALUE_STRING: {
		raise_error("< operator for the string is not defined.");
		break;
	}
	default:
		assert(false);
		break;
	}
	destroy_value(t);
}

void
value_ltoe(value_t* v, const value_t* r)
{
	value_t* t = value_convert_type(v->type_, r);
	switch (v->type_) {
	case VALUE_INT:
		value_set(v, v->ivalue_ <= t->ivalue_ ? 1 : 0);
		break;
	case VALUE_DOUBLE:
		value_set(v, v->dvalue_ <= t->dvalue_ ? 1 : 0);
		break;
	case VALUE_STRING: {
		raise_error("<= operator for the string is not defined.");
		break;
	}
	default:
		assert(false);
		break;
	}
	destroy_value(t);
}

void
value_add(value_t* v, const value_t* r)
{
	value_t* t = value_convert_type(v->type_, r);
	switch (v->type_) {
	case VALUE_INT:
		v->ivalue_ += t->ivalue_;
		break;
	case VALUE_DOUBLE:
		v->dvalue_ += t->dvalue_;
		break;
	case VALUE_STRING: {
		const int required =
			snprintf(NULL, 0, "%s%s", v->svalue_, t->svalue_);
		char* s = create_string(required);
		sprintf(s, "%s%s", v->svalue_, t->svalue_);
		destroy_string(v->svalue_);
		v->svalue_ = s;
		break;
	}
	default:
		assert(false);
		break;
	}
	destroy_value(t);
}

void
value_sub(value_t* v, const value_t* r)
{
	value_t* t = value_convert_type(v->type_, r);
	switch (v->type_) {
	case VALUE_INT:
		v->ivalue_ -= t->ivalue_;
		break;
	case VALUE_DOUBLE:
		v->dvalue_ -= t->dvalue_;
		break;
	case VALUE_STRING: {
		raise_error("- operator for the string is not defined.");
		break;
	}
	default:
		assert(false);
		break;
	}
	destroy_value(t);
}

void
value_mul(value_t* v, const value_t* r)
{
	value_t* t = value_convert_type(v->type_, r);
	switch (v->type_) {
	case VALUE_INT:
		v->ivalue_ *= t->ivalue_;
		break;
	case VALUE_DOUBLE:
		v->dvalue_ *= t->dvalue_;
		break;
	case VALUE_STRING: {
		raise_error("* operator for the string is not defined.");
		break;
	}
	default:
		assert(false);
		break;
	}
	destroy_value(t);
}

void
value_div(value_t* v, const value_t* r)
{
	value_t* t = value_convert_type(v->type_, r);
	switch (v->type_) {
	case VALUE_INT:
		v->ivalue_ /= t->ivalue_;
		break;
	case VALUE_DOUBLE:
		v->dvalue_ /= t->dvalue_;
		break;
	case VALUE_STRING: {
		raise_error("/ operator for the string is not defined.");
		break;
	}
	default:
		assert(false);
		break;
	}
	destroy_value(t);
}

void
value_mod(value_t* v, const value_t* r)
{
	value_t* t = value_convert_type(v->type_, r);
	switch (v->type_) {
	case VALUE_INT:
		v->ivalue_ %= t->ivalue_;
		break;
	case VALUE_DOUBLE: {
		raise_error("\\ operator for the double is not defined.");
		break;
	}
	case VALUE_STRING: {
		raise_error("\\ operator for the string is not defined.");
		break;
	}
	default:
		assert(false);
		break;
	}
	destroy_value(t);
}

// スタック
value_stack_t*
create_value_stack()
{
	value_stack_t* res = (value_stack_t*)xmalloc(sizeof(value_stack_t), "");
	initialize_value_stack(res);
	return res;
}

void
destroy_value_stack(value_stack_t* st)
{
	uninitialize_value_stack(st);
	xfree(st);
}

void
initialize_value_stack(value_stack_t* st)
{
	const int l = 16; // 初期サイズ
	st->stack_ = (value_t**)xmalloc(sizeof(value_t*) * l, "");
	st->top_ = 0;
	st->max_ = l;
}

void
uninitialize_value_stack(value_stack_t* st)
{
	stack_pop(st, st->top_);
	xfree(st->stack_);
	st->stack_ = NULL;
	st->top_ = 0;
	st->max_ = 0;
}

void
stack_push(value_stack_t* st, value_t* v)
{
	if (st->top_ + 1 > st->max_) {
		st->max_ = st->max_ * 2; // 貪欲
		st->stack_ = (value_t**)xrealloc(st->stack_, sizeof(value_t*) * st->max_);
	}
	st->stack_[st->top_] = v;
	++st->top_;
}

void
stack_push2(value_stack_t* st, const value_t* v)
{
	value_t* vp = create_value5(v);
	stack_push(st, vp);
}

value_t*
stack_peek(value_stack_t* st, int i)
{
	const int idx = (i < 0 ? st->top_ + i : i);
	assert(idx >= 0 && idx < st->top_);
	return st->stack_[idx];
}

void
stack_pop(value_stack_t* st, size_t n)
{
	assert(n <= (size_t)st->top_);
	while (n-- > 0) {
		--st->top_;
		assert(st->stack_[st->top_] != NULL);
		destroy_value(st->stack_[st->top_]);
		st->stack_[st->top_] = NULL;
	}
}

// システム変数
int
query_sysvar(const char* s)
{
	static const struct
	{
		int tag_;
		const char* word_;
	} table[] = {
		{
			SYSVAR_CNT, "cnt",
		},
		{
			SYSVAR_STAT, "stat",
		},
		{
			SYSVAR_REFDVAL, "refdval",
		},
		{
			SYSVAR_REFSTR, "refstr",
		},
		{
			SYSVAR_STRSIZE, "strsize",
		},
#ifdef __HSPGUI__
		{
			SYSVAR_MOUSEX, "mousex",
		},
		{
			SYSVAR_MOUSEY, "mousey",
		},
		{
			SYSVAR_MOUSEL, "mousel",
		},
		{
			SYSVAR_MOUSER, "mouser",
		},
#endif
		{ -1, NULL },
	};
	// 全探索
	for (int i = 0; table[i].tag_ != -1; ++i) {
		if (string_equal_igcase(s, table[i].word_, -1)) {
			return table[i].tag_;
		}
	}
	return -1;
}

// 実行環境
execute_environment_t*
create_execute_environment()
{
	execute_environment_t* res = (execute_environment_t*)xmalloc(sizeof(execute_environment_t), "");
	res->parser_list_ = create_list();
	res->ast_list_ = create_list();
	res->statement_list_ = create_list();
	res->label_table_ = create_list();
	res->variable_table_ = create_variable_table();
	return res;
}

void
destroy_execute_environment(execute_environment_t* e)
{
	{
		// 自分でallocしたものだけ先に消す
		list_node_t* node = e->statement_list_->head_;
		while (node != NULL) {
			ast_node_t* ast = (ast_node_t*)node->value_;
			if (ast->flag_ & NODE_FLAG_ADDITIONAL) {
				destroy_ast_node(ast);
			}
			node = node->next_;
		}
		list_free_all(e->statement_list_);
		destroy_list(e->statement_list_);
	}
	{
		list_node_t* node = e->parser_list_->head_;
		while (node != NULL) {
			parse_context_t* parser = (parse_context_t*)node->value_;
			uninitialize_parse_context(parser);
			destroy_parse_context(parser);
			node = node->next_;
		}
		list_free_all(e->parser_list_);
		destroy_list(e->parser_list_);
	}
	{
		list_node_t* node = e->ast_list_->head_;
		while (node != NULL) {
			list_t* ast = (list_t*)node->value_;
			destroy_ast(ast);
			node = node->next_;
		}
		list_free_all(e->ast_list_);
		destroy_list(e->ast_list_);
	}
	{
		list_node_t* node = e->label_table_->head_;
		while (node != NULL) {
			label_node_t* label_node = (label_node_t*)node->value_;
			xfree(label_node->name_);
			xfree(label_node);
			node->value_ = NULL;
			node = node->next_;
		}
		list_free_all(e->label_table_);
		destroy_list(e->label_table_);
	}
	destroy_variable_table(e->variable_table_);
	xfree(e);
}

void
initialize_execute_status(execute_status_t* s)
{
	s->stack_ = create_value_stack();
	s->node_cur_ = NULL;
	s->current_call_frame_ = 0;
	s->call_frame_[0].caller_ = NULL;
	s->current_loop_frame_ = 0;
	s->loop_frame_[0].counter_ = 0;
	s->loop_frame_[0].max_ = 0;
	s->loop_frame_[0].cnt_ = 0;
	s->is_end_ = false;
	s->stat_ = 0;
	s->refdval_ = 0.0;
	s->refstr_ = create_string3("");
	s->strsize_ = 0;
}

void
uninitialize_execute_status(execute_status_t* s)
{
	destroy_value_stack(s->stack_);
	destroy_string(s->refstr_);
	s->refstr_ = NULL;
}

ast_node_t*
_cl()
{
	ast_node_t* n = create_ast_node(NODE_JUMP_LABEL, NULL, NULL);
	n->flag_ |= NODE_FLAG_ADDITIONAL;
	return n;
}

ast_node_t*
_cj()
{
	ast_node_t*  n = create_ast_node(NODE_JUMP_INTERNAL, NULL, NULL);
	n->flag_ |= NODE_FLAG_ADDITIONAL;
	return n;
}

//ast_node_t*
void
_aj(list_node_t* tail, execute_environment_t* e)
{
	ast_node_t*  j = _cj();
	j->ext_ = tail;
	list_node_t* jumper = create_list_node();
	jumper->value_ = j;
	list_append(e->statement_list_, jumper);
}

static void
flatten(execute_environment_t* e, ast_node_t* node)
{
	bool is_add = true;
	// 前処理
	if (node->tag_ == NODE_BLOCK_STATEMENTS) {
		if (node->left_) {
			flatten(e, node->left_);
		}
		if (node->right_) {
			flatten(e, node->right_);
		}
		is_add = false;
	}
	else if (node->tag_ == NODE_IF) {
		// 処理用のノードを付け足す
		{
			list_node_t* check_node = create_list_node();
			ast_node_t* checker = create_ast_node(NODE_IF_CHECK, NULL, NULL);
			checker->flag_ |= NODE_FLAG_ADDITIONAL;
			checker->ext_ = node->left_;
			check_node->value_ = checker;
			list_append(e->statement_list_, check_node);
		}
		// 各ブロックを線形に貼りなおす
		{
			const ast_node_t* dispatcher = node->right_;
			assert(dispatcher->tag_ == NODE_IF_DISPATCHER);
			list_node_t* true_head = create_list_node();
			true_head->value_ = _cl();
			list_node_t* false_head = create_list_node();
			false_head->value_ = _cl();
			list_node_t* tail = create_list_node();
			tail->value_ = _cl();
			// 分岐
			_aj(true_head, e);
			_aj(false_head, e);
			// 真
			list_append(e->statement_list_, true_head);
			flatten(e, dispatcher->left_);
			_aj(tail, e);
			// 偽
			list_append(e->statement_list_, false_head);
			if (dispatcher->right_) {
				flatten(e, dispatcher->right_);
				_aj(tail, e);
			}
			// 合流
			list_append(e->statement_list_, tail);
		}
		is_add = false;
	}
	// 後処理
	if (is_add) {
		list_node_t* list_node = create_list_node();
		list_node->value_ = node;
		list_append(e->statement_list_, list_node);
		if (node->tag_ == NODE_LABEL) {
			list_node_t* label_node = create_list_node();
			label_node_t* label =
				(label_node_t*)xmalloc(sizeof(label_node_t), "");
			label->name_ = create_string3(node->token_->content_);
			label->statement_ = list_node;
			label_node->value_ = label;
			list_append(e->label_table_, label_node);
		}
		else if (node->tag_ == NODE_REPEAT) {
			// 処理用のノードを付け足す
			list_node_t* check_node = create_list_node();
			ast_node_t* checker = create_ast_node(NODE_REPEAT_CHECK, NULL, NULL);
			checker->flag_ |= NODE_FLAG_ADDITIONAL;
			check_node->value_ = checker;
			list_append(e->statement_list_, check_node);
		}
	}
}

static void
walk(execute_environment_t* e, ast_node_t* node)
{
	if (node->tag_ == NODE_VARIABLE ||
		node->tag_ == NODE_IDENTIFIER_EXPR //変数配列の可能性あり
		) {
		char* var_name = node->token_->content_;
		if (search_variable(e->variable_table_, var_name) == NULL) {
			// 適当な変数として初期化しておく
			value_t* v = (value_t*)calloc(1, sizeof(value_t));
			v->variable_ = (variable_t*)calloc(1, sizeof(variable_t));
			v->type_ = VALUE_INT;
			v->ivalue_ = 0;
			variable_set(e->variable_table_, v, var_name, 0);

		}
	}
	if (node->left_ != NULL) {
		walk(e, node->left_);
	}
	if (node->right_ != NULL) {
		walk(e, node->right_);
	}
}

void
load_script(execute_environment_t* e, const char* script)
{
	tokenize_context_t tokenizer;
	initialize_tokenize_context(&tokenizer, script);
	parse_context_t* parser = create_parse_context();
	initialize_parse_context(parser, &tokenizer);
	list_t* ast = parse_script(parser);

	uninitialize_tokenize_context(&tokenizer);

	// ASTを繋げたりラベルを持っておいたり
	{
		list_node_t* st = ast->head_;
		while (st != NULL) {
			ast_node_t* node = (ast_node_t*)st->value_;
			flatten(e, node);
			st = st->next_;
		}
	}
	// 特定の部分木マッチング
	{
		list_node_t* st = ast->head_;
		while (st != NULL) {
			ast_node_t* node = (ast_node_t*)st->value_;
			walk(e, node);
			st = st->next_;
		}
	}

	// パーサーとASTを保存しておく
	{
		list_node_t* parser_node = create_list_node();
		parser_node->value_ = parser;
		list_append(e->parser_list_, parser_node);
	}
	{
		list_node_t* ast_node = create_list_node();
		ast_node->value_ = ast;
		list_append(e->ast_list_, ast_node);
	}
}

void
evaluate(execute_environment_t* e, execute_status_t* s, ast_node_t* n)
{
	// もう実行おわってる
	if (s->is_end_) {
		return;
	}
	switch (n->tag_) {
	case NODE_EMPTY:
	case NODE_LABEL:
		break;
	case NODE_BLOCK_STATEMENTS:
		if (n->left_) {
			evaluate(e, s, n->left_);
		}
		if (n->right_) {
			evaluate(e, s, n->right_);
		}
		break;
	case NODE_COMMAND: {
		assert(n->token_->tag_ == TOKEN_IDENTIFIER);
		const char* command_name = n->token_->content_;
		const int command = query_command(command_name);
		if (command == -1) {
			raise_error("Command not found:%s", command_name);
		}
		const command_delegate delegate = get_command_delegate((builtin_command_tag)command);
		assert(delegate != NULL);
		const int top = s->stack_->top_;
		if (n->left_ != NULL) {
			evaluate(e, s, n->left_);
		}
		const int arg_num = s->stack_->top_ - top;
		delegate(e, s, arg_num);
		assert(s->stack_->top_ == top); // 戻り値がないことを確認
		break;
	}
	case NODE_ARGUMENTS: {
		if (n->left_ != NULL) {
			evaluate(e, s, n->left_);
		}
		if (n->right_ != NULL) {
			evaluate(e, s, n->right_);
		}
		break;
	}
	case NODE_ASSIGN: {
		evaluate(e, s, n->left_);
		evaluate(e, s, n->right_);
		value_t* var = stack_peek(s->stack_, -2);
		if (var->type_ != VALUE_VARIABLE) {
			raise_error("Substitution: Please specify a variable.");
		}
		value_t* v = stack_peek(s->stack_, -1);

		value_isolate(v);
		if (var->variable_ == NULL) {
			variable_set(e->variable_table_, v, NULL, var->index_);
		}
		else {
			variable_set(e->variable_table_, v, var->variable_->name_, var->index_);
		}
		stack_pop(s->stack_, 2);
		break;
	}
	case NODE_VARIABLE: {
		const char* var_name = n->token_->content_;
		variable_t* var = search_variable(e->variable_table_, var_name);
		assert(var != NULL);
		int idx = 0;
		ast_node_t* idx_node = n->left_;
		if (idx_node) {
			evaluate(e, s, idx_node);
			value_t* i = stack_peek(s->stack_, -1);
			idx = value_calc_int(i);
			stack_pop(s->stack_, 1);
		}
		value_t* v = create_value4(var, idx);
		stack_push(s->stack_, v);
		break;
	}
	case NODE_EXPRESSION:
		evaluate(e, s, n->left_);
		break;
	case NODE_BOR:
	case NODE_BAND:
	case NODE_EQ:
	case NODE_NEQ:
	case NODE_GT:
	case NODE_GTOE:
	case NODE_LT:
	case NODE_LTOE:
	case NODE_ADD:
	case NODE_SUB:
	case NODE_MUL:
	case NODE_DIV:
	case NODE_MOD: {
		evaluate(e, s, n->left_);
		evaluate(e, s, n->right_);
		value_t* l = stack_peek(s->stack_, -2);
		value_t* r = stack_peek(s->stack_, -1);
		value_isolate(l);
		switch (n->tag_) {
		case NODE_BOR:
			value_bor(l, r);
			break;
		case NODE_BAND:
			value_band(l, r);
			break;
		case NODE_EQ:
			value_eq(l, r);
			break;
		case NODE_NEQ:
			value_neq(l, r);
			break;
		case NODE_GT:
			value_gt(l, r);
			break;
		case NODE_GTOE:
			value_gtoe(l, r);
			break;
		case NODE_LT:
			value_lt(l, r);
			break;
		case NODE_LTOE:
			value_ltoe(l, r);
			break;
		case NODE_ADD:
			value_add(l, r);
			break;
		case NODE_SUB:
			value_sub(l, r);
			break;
		case NODE_MUL:
			value_mul(l, r);
			break;
		case NODE_DIV:
			value_div(l, r);
			break;
		case NODE_MOD:
			value_mod(l, r);
			break;
		default:
			assert(false);
			break;
		}
		stack_pop(s->stack_, 1);
		break;
	}
	case NODE_UNARY_MINUS: {
		assert(n->left_ != NULL);
		evaluate(e, s, n->left_);
		value_t* v = stack_peek(s->stack_, -1);
		value_isolate(v);
		switch (v->type_) {
		case VALUE_INT:
			v->ivalue_ = -v->ivalue_;
			break;
		case VALUE_DOUBLE:
			v->dvalue_ = -v->dvalue_;
			break;
		case VALUE_STRING:
			raise_error("no minus value in the string.[%s]", v->svalue_);
			break;
		default:
			assert(false);
			break;
		}
		break;
	}
	case NODE_PRIMITIVE_VALUE: {
		switch (n->token_->tag_) {
		case TOKEN_INTEGER:
			stack_push(s->stack_, create_value(atoi(n->token_->content_)));
			break;
		case TOKEN_REAL:
			stack_push(s->stack_, create_value2(atof(n->token_->content_)));
			break;
		case TOKEN_STRING:
			stack_push(s->stack_, create_value3(n->token_->content_));
			break;
		default:
			assert(false);
			break;
		}
		break;
	}
	case NODE_IDENTIFIER_EXPR: {
		assert(n->token_->tag_ == TOKEN_IDENTIFIER);
		const char* ident = n->token_->content_;
		const int top = s->stack_->top_;
		if (n->left_ != NULL) {
			evaluate(e, s, n->left_);
		}
		const int arg_num = s->stack_->top_ - top;
		const int function = query_function(ident);
		if (function >= 0) {
			// 関数呼び出し
			const function_delegate delegate =
				get_function_delegate((builtin_function_tag)function);
			assert(delegate != NULL);
			delegate(e, s, arg_num);
			assert(s->stack_->top_ == top + 1); // 戻り値が入っていることを確認する
		}
		else {
			// システム変数
			const int sysvar = query_sysvar(ident);
			if (sysvar >= 0) {
				if (arg_num > 0) {
					raise_error("cannot arrays in the system variable.", ident);
				}
				// 後々のことも考えて、一応
				stack_pop(s->stack_, arg_num);
				switch (sysvar) {
				case SYSVAR_CNT:
					if (s->current_loop_frame_ <= 0) {
						raise_error("System variable(cnt): cannot refer outside the repeat-loop.");
					}
					stack_push(s->stack_, create_value(s->loop_frame_[s->current_loop_frame_ - 1].cnt_));
					break;
				case SYSVAR_STAT:
					stack_push(s->stack_, create_value(s->stat_));
					break;
				case SYSVAR_REFDVAL:
					stack_push(s->stack_, create_value2(s->refdval_));
					break;
				case SYSVAR_REFSTR:
					stack_push(s->stack_, create_value3(s->refstr_));
					break;
				case SYSVAR_STRSIZE:
					stack_push(s->stack_, create_value(s->strsize_));
					break;
#ifdef __HSPGUI__
					case SYSVAR_MOUSEX:
						stack_push(s->stack_, create_value(current_mouse_x));
						break;
					case SYSVAR_MOUSEY:
						stack_push(s->stack_, create_value(current_mouse_y));
						break;
					case SYSVAR_MOUSEL:
						stack_push(s->stack_, create_value(current_mouse_down_left));
						break;
					case SYSVAR_MOUSER:
						stack_push(s->stack_, create_value(current_mouse_down_right));
						break;
#endif
				default:
					assert(false);
					stack_push(s->stack_, create_value(0));
					break;
				}
			}
			else {
				// 配列変数
				if (arg_num > 1) {
					raise_error("Function not found, Array variable is one dimension only.@@ %s", ident);
				}
				variable_t* var = search_variable(e->variable_table_, ident);
				assert(var != NULL);
				int idx = 0;
				if (arg_num > 0) {
					value_t* i = stack_peek(s->stack_, -1);
					idx = value_calc_int(i);
				}
				value_t* v = create_value4(var, idx);
				stack_pop(s->stack_, arg_num);
				stack_push(s->stack_, v);
			}
		}
		break;
	}
	case NODE_END:
		s->is_end_ = true;
		break;
	case NODE_RETURN: {
		if (s->current_call_frame_ <= 0) {
			raise_error("Return from outside the subroutine is invalid.");
		}
		if (n->left_) {
			evaluate(e, s, n->left_);
			const value_t* res = stack_peek(s->stack_, -1);
			switch (value_get_primitive_tag(res)) {
			case VALUE_INT:
				s->stat_ = value_calc_int(res);
				break;
			case VALUE_DOUBLE:
				s->refdval_ = value_calc_double(res);
				break;
			case VALUE_STRING:
				destroy_string(s->refstr_);
				s->refstr_ = value_calc_string(res);
				break;
			default:
				assert(false);
				break;
			}
			stack_pop(s->stack_, 1);
		}
		--s->current_call_frame_;
		s->node_cur_ = s->call_frame_[s->current_call_frame_].caller_;
		break;
	}
	case NODE_GOTO: {
		const ast_node_t* label_node = n->left_;
		assert(label_node != NULL);
		assert(label_node->tag_ == NODE_LABEL);
		const char* label_name = label_node->token_->content_;
		list_node_t* label = search_label(e, label_name);
		if (label == NULL) {
			raise_error("goto: Label not found.@@ %s", label_name);
		}
		s->node_cur_ = label;
		break;
	}
	case NODE_GOSUB: {
		const ast_node_t* label_node = n->left_;
		assert(label_node != NULL);
		assert(label_node->tag_ == NODE_LABEL);
		const char* label_name = label_node->token_->content_;
		list_node_t* label = search_label(e, label_name);
		if (label == NULL) {
			raise_error("gosub: Label not found.@@ %s", label_name);
		}
		if ((s->current_call_frame_ + 1) >= MAX_CALL_FRAME) {
			raise_error("gosub: Nesting is too deep.");
		}
		++s->current_call_frame_;
		s->call_frame_[s->current_call_frame_ - 1].caller_ = s->node_cur_;
		s->node_cur_ = label;
		break;
	}
	case NODE_REPEAT: {
		if (s->current_loop_frame_ + 1 >= MAX_LOOP_FRAME) {
			raise_error("repeat: Nesting is too deep.");
		}
		++s->current_loop_frame_;
		s->loop_frame_[s->current_loop_frame_ - 1].start_ = s->node_cur_;
		s->loop_frame_[s->current_loop_frame_ - 1].cnt_ = 0;
		s->loop_frame_[s->current_loop_frame_ - 1].counter_ = 0;
		s->loop_frame_[s->current_loop_frame_ - 1].max_ = 0;
		int loop_num = -1;
		if (n->left_) {
			evaluate(e, s, n->left_);
			const value_t* v = stack_peek(s->stack_, -1);
			loop_num = value_calc_int(v);
			stack_pop(s->stack_, 1);
		}
		s->loop_frame_[s->current_loop_frame_ - 1].max_ = loop_num;
		break;
	}
	case NODE_REPEAT_CHECK: {
		assert(s->current_loop_frame_ > 0);
		if (s->loop_frame_[s->current_loop_frame_ - 1].max_ >= 0 &&
			s->loop_frame_[s->current_loop_frame_ - 1].counter_ >= s->loop_frame_[s->current_loop_frame_ - 1].max_) {
			int depth = 0;
			while (s->node_cur_ != NULL) {
				ast_node_t* ex = (ast_node_t*)s->node_cur_->value_;
				if (ex->tag_ == NODE_REPEAT) {
					++depth;
				}
				else if (ex->tag_ == NODE_LOOP) {
					if (depth-- == 0) {
						break;
					}
				}
				s->node_cur_ = s->node_cur_->next_;
			}
			assert(s->node_cur_ != NULL);
			--s->current_loop_frame_;
		}
		break;
	}
	case NODE_LOOP:
	case NODE_CONTINUE: {
		if (s->current_loop_frame_ <= 0) {
			raise_error("loop,continue: is not in repeat-loop.");
		}
		++s->loop_frame_[s->current_loop_frame_ - 1].counter_;
		++s->loop_frame_[s->current_loop_frame_ - 1].cnt_;
		s->node_cur_ = s->loop_frame_[s->current_loop_frame_ - 1].start_;
		break;
	}
	case NODE_BREAK: {
		if (s->current_loop_frame_ <= 0) {
			raise_error("break: is not in repeat-loop.");
		}
		int depth = 0;
		while (s->node_cur_ != NULL) {
			ast_node_t* ex = (ast_node_t*)s->node_cur_->value_;
			if (ex->tag_ == NODE_REPEAT) {
				++depth;
			}
			else if (ex->tag_ == NODE_LOOP) {
				if (depth-- == 0) {
					break;
				}
			}
			s->node_cur_ = s->node_cur_->next_;
		}
		if (s->node_cur_ == NULL) {
			raise_error("break: couldn't repeat-loop in goto, and break.");
		}
		--s->current_loop_frame_;
		break;
	}
	case NODE_IF: {
		assert(n->left_ != NULL);
		evaluate(e, s, n->left_);
		assert(n->right_ != NULL);
		const ast_node_t* dispatcher = n->right_;
		assert(dispatcher->tag_ == NODE_IF_DISPATCHER);
		const value_t* cond = stack_peek(s->stack_, -1);
		const bool is_cond = value_calc_boolean(cond);
		stack_pop(s->stack_, 1);
		if (is_cond) {
			evaluate(e, s, dispatcher->left_);
		}
		else {
			if (dispatcher->right_) {
				evaluate(e, s, dispatcher->right_);
			}
		}
		break;
	}
	case NODE_IF_DISPATCHER:
		assert(false);
		break;
	case NODE_IF_CHECK: {
		assert(n->ext_ != NULL);
		ast_node_t* c = (ast_node_t*)n->ext_;
		evaluate(e, s, c);
		const value_t* cond = stack_peek(s->stack_, -1);
		const bool is_cond = value_calc_boolean(cond);
		stack_pop(s->stack_, 1);
		if (!is_cond) {
			s->node_cur_ = s->node_cur_->next_;
		}
		break;
	}
	case NODE_JUMP_LABEL:
		break;
	case NODE_JUMP_INTERNAL:
		assert(n->ext_ != NULL);
		s->node_cur_ = (list_node_t*)n->ext_;
		break;
	default:
		assert(false);
		break;
	}
	return;
}

void
execute(execute_environment_t* e)
{
	execute_status_t s;
	initialize_execute_status(&s);
	s.node_cur_ = e->statement_list_->head_;
	if (s.node_cur_ == NULL) {
		raise_error("No executable node.@@ [%p]", e);
	}
	for (;;) {
		ast_node_t* ex = (ast_node_t*)s.node_cur_->value_;
		const int top = s.stack_->top_;
		evaluate(e, &s, ex);
		assert(top == s.stack_->top_);
		if (s.is_end_) {
			// もう実行終わったらしい、帰る
			break;
		}
		s.node_cur_ = s.node_cur_->next_;
		if (s.node_cur_ == NULL) {
			// もう実行できるastがない、帰る
			break;
		}
	}
	uninitialize_execute_status(&s);
}

// ビルトイン
int
query_command(const char* s)
{
	static const struct
	{
		int tag_;
		const char* word_;
	} table[] = {
		{
			COMMAND_DIM, "dim",
		},
		{
			COMMAND_DDIM, "ddim",
		},
		{
			COMMAND_SDIM, "sdim",
		},
		{
			COMMAND_BLOAD, "bload",
		},
		{
			COMMAND_BSAVE, "bsave",
		},
		{
			COMMAND_POKE, "poke",
		},
		{
			COMMAND_RUN, "run",
		},
#ifdef __HSPCUI__
		{
			COMMAND_INPUT, "input",
		},
#endif
#ifndef __HSPSTD__
		{
			COMMAND_MES, "mes",
		},
#endif
#ifdef __HSPGUI__
		{
			COMMAND_WAIT, "wait",
		},
		{
			COMMAND_STOP, "stop",
		},
		{
			COMMAND_TITLE, "title",
		},
		{
			COMMAND_PSET, "pset",
		},
		{
			COMMAND_REDRAW, "redraw",
		},
		{
			COMMAND_POS, "pos",
		},
		{
			COMMAND_COLOR, "color",
		},
		{
			COMMAND_LINE, "line",
		},
		{
			COMMAND_BOXF, "boxf",
		},
		{
			COMMAND_STICK, "stick",
		},
		{
			COMMAND_CIRCLE, "circle",
		},
#ifdef __HSPEXT__
		{
			COMMAND_FONT, "font",
		},
		{
			COMMAND_PICLOAD, "picload",
		},
		{
			COMMAND_BEEP, "beep",
		},
#endif
#endif
		{ -1, NULL },
	};
	// 全探索
	for (int i = 0; table[i].tag_ != -1; ++i) {
		if (string_equal_igcase(s, table[i].word_, -1)) {
			return table[i].tag_;
		}
	}
	return -1;
}

command_delegate
get_command_delegate(builtin_command_tag command)
{
	static const command_delegate commands[] = {
		&command_dim,
		&command_ddim,
		&command_sdim,
		&command_bload,
		&command_bsave,
		&command_poke,
		&command_run,
#ifdef __HSPCUI__
		&command_input,
#endif
#ifndef __HSPSTD__
		&command_mes,
#endif
#ifdef __HSPGUI__
		&command_wait,
		&command_stop,
		&command_title,
		&command_pset,
		&command_redraw,
		&command_pos,
		&command_color,
		&command_line,
		&command_boxf,
		&command_stick,
		&command_circle,
#ifdef __HSPEXT__
		&command_font,
		&command_picload,
		&command_beep,
#endif
#endif
		NULL,
	};
	return commands[command];
}

int
query_function(const char* s)
{
	static const struct
	{
		int tag_;
		const char* word_;
	} table[] = {
		{
			FUNCTION_INT, "int",
		},
		{
			FUNCTION_DOUBLE, "double",
		},
		{
			FUNCTION_STR, "str",
		},
		{
			FUNCTION_RND, "rnd",
		},
		{
			FUNCTION_ABS, "abs",
		},
		{
			FUNCTION_POWF, "powf",
		},
		{
			FUNCTION_PEEK, "peek",
		},
		{ -1, NULL },
	};
	// 全探索
	for (int i = 0; table[i].tag_ != -1; ++i) {
		if (string_equal_igcase(s, table[i].word_, -1)) {
			return table[i].tag_;
		}
	}
	return -1;
}

function_delegate
get_function_delegate(builtin_function_tag function)
{
	static const function_delegate functions[] = {
		&function_int,
		&function_double,
		&function_str,
		&function_rnd,
		&function_abs,
		&function_powf,
		&function_peek,
		NULL,
	};
	return functions[function];
}

#ifdef __HSPGUI__
void
mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
	if (button == 0) { // 左クリック
		current_mouse_down_left = action;
	}
	else if (button == 1) { // 右クリック
		current_mouse_down_right = action;
	}
	else {
	}
}

void
cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	current_mouse_x = (int)xpos;
	current_mouse_y = (int)ypos;
}

void
key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_SPACE:
			break;
		case GLFW_KEY_ESCAPE:
			break;
		case GLFW_KEY_ENTER:
			break;
		case GLFW_KEY_TAB:
			break;
		case GLFW_KEY_RIGHT:
			break;
		case GLFW_KEY_LEFT:
			break;
		case GLFW_KEY_DOWN:
			break;
		case GLFW_KEY_UP:
			break;
		case GLFW_KEY_LEFT_CONTROL:
			break;
		case GLFW_KEY_RIGHT_CONTROL:
			break;
		default:
			break;
		}
	}
}
#endif

int
main(int argc, char* argv[])
//main(int argc, const char* argv[])
{

#ifdef __HSPEXT__
	// OpenALを準備する
	{
		al_device = alcOpenDevice(NULL); // デバイスを開く
		al_context = alcCreateContext(al_device, NULL); // コンテキストを作成
		alcMakeContextCurrent(al_context); // コンテキストをカレントにする
	}
#endif

#ifdef __HSPGUI__
	// 描画処理
	{
		current_color_r = 0;
		current_color_g = 0;
		current_color_b = 0;
		current_pos_x = 0;
		current_pos_y = 0;
		screen_width = 640;
		screen_height = 480;
		window_title = (char*)"Untitled";
		redraw_flag = true;
		close_flag = false;
		for (int i = 0; i < screen_width * screen_height * 3; i++) {
			pixel_data[i] = 255;
		}

		// GLFWライブラリの初期化
		{
			//初期化して、ウインドウを生成する
			glfwInit();
			window = glfwCreateWindow(screen_width,
				screen_height,
				window_title,
				NULL,
				NULL);
			glfwMakeContextCurrent(window);
		}

		// コールバック関数を登録する
		glfwSetMouseButtonCallback(window, mouse_button_callback);
		glfwSetCursorPosCallback(window, cursor_position_callback);
		glfwSetKeyCallback(window, key_callback);

		// １度だけスクリーンを初期化する
		{
			// 描画の準備
			glClear(GL_COLOR_BUFFER_BIT);
			glRasterPos2i(-1, -1);

			// ピクセルを描画
			glDrawPixels(screen_width,
				screen_height,
				GL_RGB,
				GL_UNSIGNED_BYTE,
				pixel_data);

			// フロントバッファとバックバッファを交換する
			glfwSwapBuffers(window);
		}
	}
#endif

	// オプション
	filename = "start.hs";
	if (argc >= 2) filename = argv[1];

re_run:

	// システムここから
	initialize_system();
	// ファイル読み込み
	size_t script_size = 0;
	char* script = NULL;
	{
		FILE* file = fopen(filename, "r");
		if (file == NULL) {
			printf("ERROR : cannot read such file %s\n", filename);
			return -1;
		}
		fseek(file, 0, SEEK_END);
		const size_t initial_size = ftell(file);
		size_t buffer_size = initial_size + 4; // 初期バッファ
		script = (char*)xmalloc(buffer_size + 1, "");
		fseek(file, 0, SEEK_SET);
		for (;;) {
			const int c = fgetc(file);
			if (c == EOF) {
				break;
			}
			const char ch = (char)c;
			if (buffer_size <= script_size) {
				buffer_size *= 2;
				script = (char*)xrealloc(script, buffer_size);
			}
			script[script_size++] = ch;
		}
		script[script_size] = '\0';
		fclose(file);
	}
	assert(script != NULL);

	// 乱数の初期化
	{
		unsigned int seed = (unsigned int)time(NULL);
		srand(seed);
	}
	// 実行
	{
		execute_environment_t* env = create_execute_environment();
		load_script(env, script);
		execute(env);
		destroy_execute_environment(env);
	}
	// run命令の判定
	if (run_flag) {
		run_flag = false;
		goto re_run;
	}

	// 各種解放
#ifdef __HSPGUI__
	glfwTerminate(); //GLFW
#ifdef __HSPEXT__
//	free(font_ttf_buffer); //フォントバッファを解放
	alcMakeContextCurrent(NULL); // 現在のコンテキストの解除(OpenAL)
	alcDestroyContext(al_context); // コンテキストの消去(OpenAL)
	alcCloseDevice(al_device); // デバイスを閉じる(OpenAL)
#endif
#endif

	xfree(script);
	destroy_system();
	return 0;
}
