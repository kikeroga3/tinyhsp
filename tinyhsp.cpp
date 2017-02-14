// For macOS:
// $ clang++ tinyhsp.cpp -o tinyhsp -std=c++11 -lglfw -framework OpenGL
//
// For MinGW:
// $ g++ tinyhsp.cpp -o tinyhsp -std=gnu++11 -lglfw3dll -lopengl32 -mwindows
// or -std=gnu++11
// or -std=c++0x
// or -std=gnu++0x
// or -std=c++11
//
// For Linux:
// $ g++ tinyhsp.c -o tinyhsp -std=c++11 -lm -ldl -lglfw3 -lGL -lX11 -lXxf86vm -lXrandr -lXinerama -lXcursor -lpthread -lXi
//
// This project is using GLFW
// http://www.glfw.org
// 
// This project is using neteruhsp
// https://github.com/exrd/neteruhsp

// コンフィグ
// メモリリーク発見用ユーティリティを有効化
#define NHSP_CONFIG_MEMLEAK_DETECTION (0)

//========================================================

#include <cassert>
#include <cstdarg>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif
#include <GLFW/glfw3.h>

// グローバルな変数
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
GLFWwindow* window;
bool redraw_flag;
bool close_flag;

// 全体
void initialize_system();
void destroy_system();

// メモリ
void* xmalloc(size_t size, const char* block_name = nullptr);
void xfree(void* ptr);
void* xrealloc(void* ptr, size_t size);

// リスト
struct list_node_t
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
struct list_t
{
	list_node_t* head_;
	list_node_t* tail_;
};

list_t* create_list();
void destroy_list(list_t* list);
void list_prepend(list_t& list, list_node_t* node);
void list_append(list_t& list, list_node_t* node);
void list_erase(list_t& list, list_node_t* node);
list_node_t* list_find(list_t& list, void* value);
void list_free_all(list_t& list);

// キーワード
enum keyword_tag
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
};

int query_keyword(const char* s);

// トークナイザ
enum token_tag
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
};

struct token_t
{
	token_tag tag_;
	char* content_;
	int cursor_begin_, cursor_end_;
	int appear_line_;
	bool left_space_, right_space_;
};

struct tokenize_context_t
{
	const char* script_;
	int cursor_;
	int line_;
	const char* line_head_;
};

int query_token_shadow(const char* ident, size_t len);
void initialize_tokenize_context(tokenize_context_t* c, const char* script);
void uninitialize_tokenize_context(tokenize_context_t* c);
token_t* get_token(tokenize_context_t& c);
void destroy_token(token_t* t);
char* create_token_string(const char* str, size_t len);

// パーサ
enum node_tag
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
};

enum ast_node_flag_tag
{
	NODE_FLAG_ADDITIONAL = 1,
};

struct ast_node_t
{
	node_tag tag_;
	token_t* token_;
	ast_node_t *left_, *right_;
	unsigned int flag_;
	void* ext_;
};

struct parse_context_t
{
	list_t* token_list_;
	list_node_t* token_current_;
	tokenize_context_t* tokenize_context_;
};

parse_context_t* create_parse_context();
void destroy_parse_context(parse_context_t* p);
void initialize_parse_context(parse_context_t* c, tokenize_context_t& t);
void uninitialize_parse_context(parse_context_t* c);
token_t* read_token(parse_context_t& c);
void unread_token(parse_context_t& c, size_t num = 1);
token_t* prev_token(parse_context_t& c, size_t num = 0);
ast_node_t* create_ast_node(node_tag tag, ast_node_t* left = nullptr, ast_node_t* right = nullptr);
ast_node_t* create_ast_node(node_tag tag, token_t* token, ast_node_t* left = nullptr);
void destroy_ast_node(ast_node_t* node);
bool is_eos_like_token(token_tag tag);
list_t* parse_script(parse_context_t& c);
void destroy_ast(list_t* ast);
ast_node_t* parse_statement(parse_context_t& c);
ast_node_t* parse_label_safe(parse_context_t& c);
ast_node_t* parse_control_safe(parse_context_t& c);
ast_node_t* parse_command_safe(parse_context_t& c);
ast_node_t* parse_arguments(parse_context_t& c);
ast_node_t* parse_assign_safe(parse_context_t& c);
ast_node_t* parse_variable_safe(parse_context_t& c);
ast_node_t* parse_expression(parse_context_t& c);
ast_node_t* parse_borand(parse_context_t& c);
ast_node_t* parse_eqneq(parse_context_t& c);
ast_node_t* parse_gtlt(parse_context_t& c);
ast_node_t* parse_addsub(parse_context_t& c);
ast_node_t* parse_muldivmod(parse_context_t& c);
ast_node_t* parse_term(parse_context_t& c);
ast_node_t* parse_primitive(parse_context_t& c);
ast_node_t* parse_identifier_expression(parse_context_t& c);

// 変数
enum value_tag
{
	VALUE_NONE,
	VALUE_INT,
	VALUE_DOUBLE,
	VALUE_STRING,
	VALUE_VARIABLE,
};

struct value_t;
struct variable_t
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
void variable_set(list_t* table, const value_t& v, const char* name, int idx);
void* variable_data_ptr(const variable_t& v, int idx);
int variable_calc_int(const variable_t& r, int idx);
double variable_calc_double(const variable_t& r, int idx);
char* variable_calc_string(const variable_t& r, int idx);

// 値（即値）
struct value_t
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

value_t* create_value(int v);
value_t* create_value(double v);
value_t* create_value(const char* v);
value_t* create_value(variable_t* v, int idx);
value_t* create_value(const value_t& v);
value_t* create_value_move(char* v);
void destroy_value(value_t* t);
void value_set(value_t* v, int i);
void value_set(value_t* v, double d);
void value_set(value_t* v, const char* s);
void value_move(value_t* v, char* s);
void value_move(value_t* to, value_t* from);
value_tag value_get_primitive_tag(const value_t& r);
bool value_calc_boolean(const value_t& r);
int value_calc_int(const value_t& r);
double value_calc_double(const value_t& r);
char* value_calc_string(const value_t& r);
value_t* value_convert_type(value_tag to, const value_t& r);
void value_isolate(value_t& v);
void value_bor(value_t* v, const value_t& r);
void value_band(value_t* v, const value_t& r);
void value_eq(value_t* v, const value_t& r);
void value_neq(value_t* v, const value_t& r);
void value_gt(value_t* v, const value_t& r);
void value_gtoe(value_t* v, const value_t& r);
void value_lt(value_t* v, const value_t& r);
void value_ltoe(value_t* v, const value_t& r);
void value_add(value_t* v, const value_t& r);
void value_sub(value_t* v, const value_t& r);
void value_mul(value_t* v, const value_t& r);
void value_div(value_t* v, const value_t& r);
void value_mod(value_t* v, const value_t& r);

// スタック
struct value_stack_t
{
	value_t** stack_;
	int top_;
	int max_;
};

value_stack_t* create_value_stack();
void destroy_value_stack(value_stack_t* st);
void initialize_value_stack(value_stack_t* st);
void uninitialize_value_stack(value_stack_t* st);
void stack_push(value_stack_t* st, value_t* v);
void stack_push(value_stack_t* st, const value_t& v);
value_t* stack_peek(value_stack_t* st, int i = -1);
void stack_pop(value_stack_t* st, size_t n = 1);

// システム変数
enum sysvar_tag
{
	SYSVAR_CNT,
	SYSVAR_STAT,
	SYSVAR_REFDVAL,
	SYSVAR_REFSTR,
	SYSVAR_MOUSEX,
	SYSVAR_MOUSEY,
	SYSVAR_MOUSEL,
	SYSVAR_MOUSER,
	MAX_SYSVAR,
};

int query_sysvar(const char* s);

// 実行環境
struct label_node_t
{
	char* name_;
	list_node_t* statement_;
};

struct call_frame_t
{
	list_node_t* caller_;
};

static const size_t MAX_CALL_FRAME = 16;
struct loop_frame_t
{
	list_node_t* start_;
	int counter_;
	int max_;
	int cnt_;
};

static const size_t MAX_LOOP_FRAME = 16;
struct execute_environment_t
{
	list_t* parser_list_;
	list_t* ast_list_;
	list_t* statement_list_;
	list_t* label_table_;
	list_t* variable_table_;
};

struct execute_status_t
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
typedef void (*command_delegate)(execute_environment_t* e, execute_status_t* s, int arg_num);
enum builtin_command_tag
{
	COMMAND_DEVTERM = 0, // デバッグ用の隠し
	COMMAND_DIM,
	COMMAND_DDIM,
	COMMAND_SDIM,
	COMMAND_RANDOMIZE,
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
	MAX_COMMAND,
};

int query_command(const char* s);
command_delegate get_command_delegate(builtin_command_tag command);
typedef void (*function_delegate)(execute_environment_t* e, execute_status_t* s, int arg_num);
enum builtin_function_tag
{
	FUNCTION_INT = 0,
	FUNCTION_DOUBLE,
	FUNCTION_STR,
	FUNCTION_RND,
	FUNCTION_ABS,
	MAX_FUNCTION,
};

int query_function(const char* s);
function_delegate get_function_delegate(builtin_function_tag command);

//========================================================
// 描画系ユーティリティ
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
	pixel_data[index+1] = color_green;
	pixel_data[index+2] = color_blue;
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
		for (int i=0; i <= deltax; ++i) {
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
		for (int i=0; i <= deltay; ++i) {
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
	int minx = MIN(start_point_x,end_point_x);
	int miny = MIN(start_point_y,end_point_y);
	int diffx = MAX(start_point_x,end_point_x) - MIN(start_point_x,end_point_x) + 1;
	int diffy = MAX(start_point_y,end_point_y) - MIN(start_point_y,end_point_y) + 1;
	for (int y=0; y<diffy; y++) {
		for (int x=0; x<diffx; x++) {
			set_pixel_rgb(pixel_data,
						  minx+x, miny+y,
						  color_red, color_green, color_blue,
						  canvas_size_width, canvas_size_height);
		}
	}
}

void redraw()
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

// エラー
void
raise_error(const char* message, ...)
{
	va_list args;
	va_start(args, message);
	vfprintf(stderr, message, args);
	va_end(args);
	printf("\n");
	exit(-1);
}

// 文字列
char*
create_string(size_t len)
{
	return reinterpret_cast<char*>(xmalloc(len + 1));
}

char*
create_string(const char* s, size_t len)
{
	char* res = create_string(len);
	strncpy(res, s, len);
	res[len] = '\0';
	return res;
}

char*
create_string(const char* s)
{
	return create_string(s, strlen(s));
}

void
destroy_string(char* s)
{
	xfree(s);
}

char*
create_string_from(int v)
{
	const auto len = snprintf(nullptr, 0, "%d", v);
	auto res = create_string(len);
	sprintf(res, "%d", v);
	return res;
}

char*
create_string_from(double v)
{
	const auto len = snprintf(nullptr, 0, "%lf", v);
	auto res = create_string(len);
	sprintf(res, "%lf", v);
	return res;
}

bool
string_equal_igcase(const char* sl, const char* r, int len = -1)
{
	const auto tol = [](char c) -> int {
		if (c >= 'A' && c <= 'Z') {
			return c - 'A' + 'a';
		}
		return c;
	};
	int iter = 0;
	for (;;) {
		if (sl[0] == '\0' || r[0] == '\0') {
			if (len < 0) {
				if (sl[0] != r[0]) {
					return false;
				}
			} else {
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
	auto node = e->label_table_->head_;
	while (node != nullptr) {
		const auto label = reinterpret_cast<label_node_t*>(node->value_);
		if (string_equal_igcase(label->name_, name)) {
			return label->statement_;
		}
		node = node->next_;
	}
	return nullptr;
}

// コマンド実体
void
command_devterm(execute_environment_t* NHSP_UNUA(e), execute_status_t* s, int arg_num)
{
	stack_pop(s->stack_, arg_num);
}

void
command_dim(execute_environment_t* NHSP_UNUA(e), execute_status_t* s, int arg_num)
{
	if (arg_num != 2) {
		raise_error("dim: Array variable is one dimension only.");
	}
	const auto arg_start = -arg_num;
	const auto v = stack_peek(s->stack_, arg_start);
	if (v->type_ != VALUE_VARIABLE) {
		raise_error("dim: Argument should be a variable.");
	}
	if (v->index_ > 0) {
		raise_error("dim: Array variables cannot be specified.");
	}
	const auto n = stack_peek(s->stack_, arg_start + 1);
	const auto num = value_calc_int(*n);
	if (num <= 0) {
		raise_error("dim: Invalid value.");
	}
	prepare_variable(v->variable_, VALUE_INT, 64, num);
	stack_pop(s->stack_, arg_num);
}

void
command_ddim(execute_environment_t* NHSP_UNUA(e), execute_status_t* s, int arg_num)
{
	if (arg_num != 2) {
		raise_error("ddim: Array variable is one dimension only.");
	}
	const auto arg_start = -arg_num;
	const auto v = stack_peek(s->stack_, arg_start);
	if (v->type_ != VALUE_VARIABLE) {
		raise_error("ddim: Argument should be a variable.");
	}
	if (v->index_ > 0) {
		raise_error("ddim: Array variables cannot be specified.");
	}
	const auto n = stack_peek(s->stack_, arg_start + 1);
	const auto num = value_calc_int(*n);
	if (num <= 0) {
		raise_error("ddim: Invalid value.");
	}
	prepare_variable(v->variable_, VALUE_DOUBLE, 64, num);
	stack_pop(s->stack_, arg_num);
}

void
command_sdim(execute_environment_t* NHSP_UNUA(e), execute_status_t* s, int arg_num)
{
	if (arg_num != 2) {
		raise_error("sdim: Array variable is one dimension only.");
	}
	const auto arg_start = -arg_num;
	const auto v = stack_peek(s->stack_, arg_start);
	if (v->type_ != VALUE_VARIABLE) {
		raise_error("sdim: Argument should be a variable.");
	}
	if (v->index_ > 0) {
		raise_error("sdim: Array variables cannot be specified.");
	}
	const auto g = stack_peek(s->stack_, arg_start + 1);
	const auto granule = value_calc_int(*g);
	const auto num = (arg_num > 2 ? value_calc_int(*stack_peek(s->stack_, arg_start + 2)) : 1);
	if (granule <= 0 || num <= 0) {
		raise_error("sdim: Invalid value.");
	}
	prepare_variable(v->variable_, VALUE_STRING, granule, num);
	stack_pop(s->stack_, arg_num);
}

void
command_randomize(execute_environment_t* NHSP_UNUA(e), execute_status_t* s, int arg_num)
{
	if (arg_num > 1) {
		raise_error("randomize: Invalid argument.");
	}
	unsigned int seed = 0;
	if (arg_num == 0) {
		seed = static_cast<unsigned int>(time(nullptr));
	} else {
		const auto m = stack_peek(s->stack_);
		seed = value_calc_int(*m);
	}
	srand(seed);
	stack_pop(s->stack_, arg_num);
}

void
command_wait(execute_environment_t* NHSP_UNUA(e), execute_status_t* s, int arg_num)
{
	if (arg_num != 1) {
		raise_error("wait: Invalid argument.");
	}
	double wait_time = 0.0;
	const auto m = stack_peek(s->stack_);
	wait_time = value_calc_double(*m);
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
command_stop(execute_environment_t* NHSP_UNUA(e), execute_status_t* s, int arg_num)
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
command_title(execute_environment_t* NHSP_UNUA(e), execute_status_t* s, int arg_num)
{
	if (arg_num != 1) {
		raise_error("title: Invalid argument.");
	}
	const auto m = stack_peek(s->stack_);
	value_isolate(*m);
	if (m->type_ != VALUE_STRING) {
		raise_error("title: Argument should specify a string type.");
	}
	glfwSetWindowTitle(window, m->svalue_);
	stack_pop(s->stack_, arg_num);
}

void
command_pset(execute_environment_t* NHSP_UNUA(e), execute_status_t* s, int arg_num)
{
	if (arg_num > 2) {
		raise_error("pset: Invalid argument.");
	}
	if (arg_num <= 0) { // 引数が省略された
		set_pixel_rgb(pixel_data,
					  current_pos_x, current_pos_y,
					  current_color_r, current_color_g, current_color_b,
					  screen_width, screen_height);
	}
	else {
		const auto arg_start = -arg_num;
		const auto p1 = stack_peek(s->stack_, arg_start);
		const auto x = value_calc_int(*p1);
		const auto p2 = stack_peek(s->stack_, arg_start + 1);
		const auto y = value_calc_int(*p2);
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
command_redraw(execute_environment_t* NHSP_UNUA(e), execute_status_t* s, int arg_num)
{
	if (arg_num > 1) {
		raise_error("redraw: Invalid argument.");
	}
	if (arg_num <= 0) { // 引数が省略された
		redraw_flag = true;
		redraw();
	}
	int redraw_flag_num = 0;
	const auto n = stack_peek(s->stack_);
	redraw_flag_num = value_calc_int(*n);
	if (redraw_flag_num == 0) {
		redraw_flag = false;
	} else if (redraw_flag_num == 1) {
		redraw_flag = true;
		redraw();
	} else {
		raise_error("redraw: Argument values only 0 or 1.");
	}
	stack_pop(s->stack_, arg_num);
}

void
command_pos(execute_environment_t* NHSP_UNUA(e), execute_status_t* s, int arg_num)
{
	if (arg_num != 2) {
		raise_error("pos: Invalid argument.");
	}
	const auto arg_start = -arg_num;
	const auto p1 = stack_peek(s->stack_, arg_start);
	const auto x = value_calc_int(*p1);
	const auto p2 = stack_peek(s->stack_, arg_start + 1);
	const auto y = value_calc_int(*p2);
	current_pos_x = x;
	current_pos_y = y;
	stack_pop(s->stack_, arg_num);
}

void
command_color(execute_environment_t* NHSP_UNUA(e), execute_status_t* s, int arg_num)
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
		const auto arg_start = -arg_num;
		const auto p1 = stack_peek(s->stack_, arg_start);
		const auto r = value_calc_int(*p1);
		const auto p2 = stack_peek(s->stack_, arg_start + 1);
		const auto g = value_calc_int(*p2);
		const auto p3 = stack_peek(s->stack_, arg_start + 2);
		const auto b = value_calc_int(*p3);
		current_color_r = r;
		current_color_g = g;
		current_color_b = b;
	}
	stack_pop(s->stack_, arg_num);
}

void
command_line(execute_environment_t* NHSP_UNUA(e), execute_status_t* s, int arg_num)
{
	if (arg_num != 4) {
		raise_error("line: Invalid argument.");
	}
	const auto arg_start = -arg_num;
	const auto p1 = stack_peek(s->stack_, arg_start);
	const auto sx = value_calc_int(*p1);
	const auto p2 = stack_peek(s->stack_, arg_start + 1);
	const auto sy = value_calc_int(*p2);
	const auto p3 = stack_peek(s->stack_, arg_start + 2);
	const auto ex = value_calc_int(*p3);
	const auto p4 = stack_peek(s->stack_, arg_start + 3);
	const auto ey = value_calc_int(*p4);
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
command_boxf(execute_environment_t* NHSP_UNUA(e), execute_status_t* s, int arg_num)
{
	if (arg_num != 4) {
		raise_error("line: Invalid argument.");
	}
	const auto arg_start = -arg_num;
	const auto p1 = stack_peek(s->stack_, arg_start);
	const auto x0 = value_calc_int(*p1);
	const auto p2 = stack_peek(s->stack_, arg_start + 1);
	const auto y0 = value_calc_int(*p2);
	const auto p3 = stack_peek(s->stack_, arg_start + 2);
	const auto x1 = value_calc_int(*p3);
	const auto p4 = stack_peek(s->stack_, arg_start + 3);
	const auto y1 = value_calc_int(*p4);
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
command_stick(execute_environment_t* NHSP_UNUA(e), execute_status_t* s, int arg_num)
{
	if (arg_num < 1 || arg_num > 2) {
		raise_error("stick: Invalid argument.");
	}
	const auto arg_start = -arg_num;
	const auto v = stack_peek(s->stack_, arg_start);
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
	reinterpret_cast<int*>(data_ptr)[0] = key;
	//prepare_variable(v->variable_, VALUE_INT, 64, num);
	stack_pop(s->stack_, arg_num);
}

// 関数実体
void
function_int(execute_environment_t* NHSP_UNUA(e), execute_status_t* s, int arg_num)
{
	if (arg_num != 1) {
		raise_error("int: Invalid argument.");
	}
	const auto m = stack_peek(s->stack_);
	const auto r = value_calc_int(*m);
	stack_pop(s->stack_, arg_num);
	stack_push(s->stack_, create_value(r));
}

void
function_double(execute_environment_t* NHSP_UNUA(e), execute_status_t* s, int arg_num)
{
	if (arg_num != 1) {
		raise_error("double: Invalid argument.");
	}
	const auto m = stack_peek(s->stack_);
	const auto r = value_calc_double(*m);
	stack_pop(s->stack_, arg_num);
	stack_push(s->stack_, create_value(r));
}

void
function_str(execute_environment_t* NHSP_UNUA(e), execute_status_t* s, int arg_num)
{
	if (arg_num != 1) {
		raise_error("str: Invalid argument.");
	}
	const auto m = stack_peek(s->stack_);
	const auto r = value_calc_string(*m);
	stack_pop(s->stack_, arg_num);
	stack_push(s->stack_, create_value_move(r));
}

void
function_rnd(execute_environment_t* NHSP_UNUA(e), execute_status_t* s, int arg_num)
{
	if (arg_num != 1) {
		raise_error("rnd: Invalid argument.");
	}
	const auto m = stack_peek(s->stack_);
	const auto r = value_calc_int(*m);
	if (r < 1) {
		raise_error("rnd: Please specify 1 or more.@@ %d", r);
	}
	stack_pop(s->stack_, arg_num);
	const auto res = rand() % (r);
	stack_push(s->stack_, create_value(res));
}

void
function_abs(execute_environment_t* NHSP_UNUA(e), execute_status_t* s, int arg_num)
{
	if (arg_num != 1) {
		raise_error("abs: Invalid argument.");
	}
	const auto m = stack_peek(s->stack_);
	const auto r = value_calc_int(*m);
	stack_pop(s->stack_, arg_num);
	const auto res = (r < 0 ? -r : r);
	stack_push(s->stack_, create_value(res));
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
	const auto res = zmalloc(size);
#if NHSP_CONFIG_MEMLEAK_DETECTION
	{
		// printf( "xmalloc[%p] %s\n", res, block_name==nullptr?"undef":"" );
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
		// printf( "xfree[%p]\n", ptr );
		zunregister_memory(ptr);
	}
#endif
	zfree(ptr);
}

void*
xrealloc(void* ptr, size_t size)
{
	const auto res = zrealloc(ptr, size);
#if NHSP_CONFIG_MEMLEAK_DETECTION
	{
		// printf( "xrealloc[%p->%p]\n", ptr, res );
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
		reinterpret_cast<list_node_t*>(xmalloc(sizeof(list_node_t)));
	res->prev_ = res->next_ = nullptr;
	res->value_ = nullptr;
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
	assert(node->prev_ == nullptr && node->next_ == nullptr);
	node->prev_ = list;
	node->next_ = list->next_;
	list->next_ = node;
}

void
link_prev(list_node_t* node, list_node_t* list)
{
	assert(node->prev_ == nullptr && node->next_ == nullptr);
	node->prev_ = list->prev_;
	node->next_ = list;
	list->prev_ = node;
}

bool
unlink_list_node(list_node_t* node)
{
	if (node->prev_ == nullptr && node->next_ == nullptr) {
		return false;
	}
	if (node->prev_ != nullptr) {
		node->prev_->next_ = node->next_;
	}
	if (node->next_ != nullptr) {
		node->next_->prev_ = node->prev_;
	}
	node->prev_ = node->next_ = nullptr;
	return true;
}

list_t*
create_list()
{
	auto res = reinterpret_cast<list_t*>(xmalloc(sizeof(list_t)));
	res->head_ = res->tail_ = nullptr;
	return res;
}

void
destroy_list(list_t* list)
{
	xfree(list);
}

void
list_prepend(list_t& list, list_node_t* node)
{
	if (list.head_ == nullptr) {
		assert(list.tail_ == nullptr);
		list.head_ = list.tail_ = node;
		return;
	}
	link_prev(node, list.head_);
	list.head_ = node;
}

void
list_append(list_t& list, list_node_t* node)
{
	if (list.tail_ == nullptr) {
		assert(list.head_ == nullptr);
		list.head_ = list.tail_ = node;
		return;
	}
	link_next(node, list.tail_);
	list.tail_ = node;
}

void
list_erase(list_t& list, list_node_t* node)
{
	if (list.head_ == node) {
		list.head_ = node->next_;
	}
	if (list.tail_ == node) {
		list.tail_ = node->prev_;
	}
	unlink_list_node(node);
}

list_node_t*
list_find(list_t& list, void* value)
{
	auto node = list.head_;
	while (node != nullptr) {
		if (node->value_ == value) {
			return node;
		}
		node = node->next_;
	}
	return nullptr;
}

void
list_free_all(list_t& list)
{
	auto node = list.head_;
	while (node != nullptr) {
		auto next = node->next_;
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
		{ -1, nullptr },
	};
	// 全探索
	for (int i = 0; table[i].tag_ != -1; ++i) {
		if (string_equal_igcase(s, table[i].word_)) {
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
		{ TOKEN_OP_BAND, "and" }, { TOKEN_OP_BOR, "or" }, { -1, nullptr },
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
	c->script_ = nullptr;
	c->cursor_ = 0;
	c->line_ = 0;
	c->line_head_ = nullptr;
}

token_t*
get_token(tokenize_context_t& c)
{
	token_t* res = reinterpret_cast<token_t*>(xmalloc(sizeof(token_t)));
	res->tag_ = TOKEN_UNKNOWN;
	res->content_ = nullptr;
	res->cursor_begin_ = c.cursor_;
	res->cursor_end_ = c.cursor_;
	res->left_space_ = false;
	res->right_space_ = false;
	const auto is_space = [](char c) { return (c == ' ' || c == '\t'); };
	const auto is_number = [](char c) { return (c >= '0' && c <= '9'); };
	const auto is_alpha = [](char c) {
		return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
	};
	const auto is_rest_ident = [&](char c) {
		return (is_number(c) || is_alpha(c) || c == '_');
	};
	const auto pp = c.script_ + c.cursor_;
	const char* p = c.script_ + c.cursor_;
restart:
	const auto prev_p = p;
	const auto prev_cursor = p - c.script_;
	res->appear_line_ = c.line_;
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
			++c.line_;
			++p;
			c.line_head_ = p;
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
			} else {
				res->tag_ = TOKEN_OP_GT;
			}
			break;
		case '<':
			++p;
			if (p[0] == '=') {
				++p;
				res->tag_ = TOKEN_OP_LTOE;
			} else {
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
			} else {
				res->tag_ = TOKEN_ASSIGN;
			}
			break;
		// 文字列
		case '\"': {
			++p;
			const auto s = p;
			while (p[0] != '\"') {
				if (p[0] == '\0') {
					raise_error("EOF detected in string.@@ %d Row", c.line_);
				}
				if (p[0] == '\\' && p[1] == '\"') {
					++p;
				}
				++p;
			}
			const auto e = p;
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
			} else if (is_number(p[0])) {
				// 数値
				if (p[0] == '0') {
					++p;
				} else {
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
				} else {
					res->tag_ = TOKEN_INTEGER;
				}
			} else if (is_alpha(p[0])) {
				// 何らかの識別子
				++p;
				while (is_rest_ident(p[0])) {
					++p;
				}
				res->tag_ = TOKEN_IDENTIFIER;
				const auto shadow =
					query_token_shadow(c.script_ + prev_cursor, p - prev_p);
				if (shadow != -1) {
					res->tag_ = static_cast<token_tag>(shadow);
				}
			} else {
				// もう読めない
				raise_error("Unknown character[%c]@@ %d Row", p[0], c.line_);
			}
			break;
	}
	if (is_space(p[0])) {
		res->right_space_ = true;
	}
	c.cursor_ += p - pp;
	res->cursor_begin_ = prev_cursor;
	res->cursor_end_ = c.cursor_;
	// contentが埋まってないなら埋める
	if (res->content_ == nullptr) {
		const auto len = res->cursor_end_ - res->cursor_begin_;
		res->content_ = create_string(len);
		memcpy(res->content_, c.script_ + res->cursor_begin_, len);
		res->content_[len] = '\0';
	}
	return res;
}

void
destroy_token(token_t* t)
{
	if (t->content_ != nullptr) {
		destroy_string(t->content_);
		t->content_ = nullptr;
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
		} else {
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
	auto res =
		reinterpret_cast<parse_context_t*>(xmalloc(sizeof(parse_context_t)));
	return res;
}

void
destroy_parse_context(parse_context_t* p)
{
	xfree(p);
}

void
initialize_parse_context(parse_context_t* c, tokenize_context_t& t)
{
	c->token_list_ = create_list();
	c->token_current_ = nullptr;
	c->tokenize_context_ = &t;
}

void
uninitialize_parse_context(parse_context_t* c)
{
	if (c->token_list_ != nullptr) {
		list_node_t* iter = c->token_list_->head_;
		while (iter != nullptr) {
			const auto next = iter->next_;
			token_t* token = reinterpret_cast<token_t*>(iter->value_);
			destroy_token(token);
			destroy_list_node(iter);
			iter = next;
		}
		destroy_list(c->token_list_);
	}
	c->token_list_ = nullptr;
	c->token_current_ = nullptr;
}

token_t*
read_token(parse_context_t& c)
{
	const auto read = [&c]() {
		auto list = create_list_node();
		list->value_ = get_token(*c.tokenize_context_);
		return list;
	};
	if (c.token_current_ == nullptr) {
		const auto node = read();
		list_append(*c.token_list_, node);
		c.token_current_ = node;
	}
	assert(c.token_current_ != nullptr);
	const auto res = c.token_current_;
	c.token_current_ = c.token_current_->next_;
	return reinterpret_cast<token_t*>(res->value_);
}

void
unread_token(parse_context_t& c, size_t num)
{
	if (num <= 0) {
		return;
	}
	if (c.token_current_ == nullptr) {
		c.token_current_ = c.token_list_->tail_;
		--num;
	}
	while (num-- > 0) {
		assert(c.token_current_ != nullptr);
		c.token_current_ = c.token_current_->prev_;
	}
}

token_t*
prev_token(parse_context_t& c, size_t num)
{
	auto current = c.token_current_;
	if (current == nullptr) {
		current = c.token_list_->tail_;
	}
	while (num-- > 0) {
		assert(current != nullptr);
		current = current->prev_;
	}
	return reinterpret_cast<token_t*>(current->value_);
}

ast_node_t*
create_ast_node(node_tag tag, ast_node_t* left, ast_node_t* right)
{
	auto res = reinterpret_cast<ast_node_t*>(xmalloc(sizeof(ast_node_t)));
	res->tag_ = tag;
	res->token_ = nullptr;
	res->left_ = left;
	res->right_ = right;
	res->flag_ = 0;
	res->ext_ = nullptr;
	return res;
}

ast_node_t*
create_ast_node(node_tag tag, token_t* token, ast_node_t* left)
{
	auto res = reinterpret_cast<ast_node_t*>(xmalloc(sizeof(ast_node_t)));
	res->tag_ = tag;
	res->token_ = token;
	res->left_ = left;
	res->right_ = nullptr;
	res->flag_ = 0;
	res->ext_ = nullptr;
	return res;
}

void
destroy_ast_node(ast_node_t* node)
{
	assert(node != nullptr);
	if (node->left_ != nullptr) {
		destroy_ast_node(node->left_);
	}
	if (node->right_ != nullptr) {
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
parse_script(parse_context_t& c)
{
	auto res = create_list();
	for (;;) {
		auto statement = parse_statement(c);
		if (statement == nullptr) {
			break;
		}
		auto node = create_list_node();
		node->value_ = statement;
		list_append(*res, node);
	}
	{
		const auto token = read_token(c);
		if (token->tag_ != TOKEN_EOF) {
			raise_error("Script couldn't be parsed successfully.@@ %d Row", token->appear_line_);
		}
	}
	return res;
}

void
destroy_ast(list_t* ast)
{
	auto node = ast->head_;
	while (node != nullptr) {
		const auto ast_node = reinterpret_cast<ast_node_t*>(node->value_);
		destroy_ast_node(ast_node);
		node = node->next_;
	}
	list_free_all(*ast);
	destroy_list(ast);
}

ast_node_t*
parse_statement(parse_context_t& c)
{
	// 何もない？
	{
		const auto token = read_token(c);
		if (token->tag_ == TOKEN_EOF) {
			return nullptr;
		}
		if (is_eos_like_token(token->tag_)) {
			return create_ast_node(NODE_EMPTY);
		}
		unread_token(c);
	}
	ast_node_t* statement = nullptr;
	// ラベルを試す
	if (statement == nullptr) {
		statement = parse_label_safe(c);
	}
	// 制御構文を試す
	if (statement == nullptr) {
		statement = parse_control_safe(c);
	}
	// コマンドを試す
	if (statement == nullptr) {
		statement = parse_command_safe(c);
	}
	// それ以外は全て代入
	if (statement == nullptr) {
		statement = parse_assign_safe(c);
	}
	// ここまで来て何もないなら、パース不能
	if (statement == nullptr) {
		const auto token = read_token(c);
		raise_error("Statement cannot be parsed.@@ %d Row", token->appear_line_);
	}
	// 最後の改行チェック
	const auto token = read_token(c);
	if (!is_eos_like_token(token->tag_)) {
		raise_error("Statement couldn't be parsed successfully.@@ %d Row", token->appear_line_);
	}
	return statement;
}

ast_node_t*
parse_label_safe(parse_context_t& c)
{
	const auto token = read_token(c);
	if (token->tag_ != TOKEN_OP_MUL) {
		unread_token(c);
		return nullptr;
	}
	const auto ident = read_token(c);
	if (ident->tag_ != TOKEN_IDENTIFIER) {
		unread_token(c, 2);
		return nullptr;
	}
	return create_ast_node(NODE_LABEL, ident);
}

ast_node_t*
parse_control_safe(parse_context_t& c)
{
	const auto ident = read_token(c);
	if (ident->tag_ != TOKEN_IDENTIFIER) {
		unread_token(c);
		return nullptr;
	}
	const auto keyword = query_keyword(ident->content_);
	if (keyword < 0) {
		unread_token(c);
		return nullptr;
	}
	switch (keyword) {
		case KEYWORD_END:
			return create_ast_node(NODE_END);
		case KEYWORD_RETURN: {
			const auto next = read_token(c);
			unread_token(c);
			ast_node_t* expr = nullptr;
			if (!is_eos_like_token(next->tag_)) {
				expr = parse_expression(c);
			}
			return create_ast_node(NODE_RETURN, expr);
		}
		case KEYWORD_GOTO:
		case KEYWORD_GOSUB: {
			const auto label = parse_label_safe(c);
			if (label == nullptr) {
				raise_error("Labels must be specified for goto or gosub.@@ %d Row", ident->appear_line_);
			}
			return create_ast_node(keyword == KEYWORD_GOTO ? NODE_GOTO : NODE_GOSUB, label);
		}
		case KEYWORD_REPEAT: {
			const auto next = read_token(c);
			unread_token(c);
			ast_node_t* expr = nullptr;
			if (!is_eos_like_token(next->tag_)) {
				expr = parse_expression(c);
			}
			return create_ast_node(NODE_REPEAT, expr);
		}
		case KEYWORD_LOOP:
			return create_ast_node(NODE_LOOP);
		case KEYWORD_CONTINUE:
			return create_ast_node(NODE_CONTINUE);
		case KEYWORD_BREAK:
			return create_ast_node(NODE_BREAK);
		case KEYWORD_IF: {
			const auto expr = parse_expression(c);
			const auto next = read_token(c);
			const auto is_else_token = [](const token_t* n) {
				if (n->tag_ == TOKEN_IDENTIFIER &&
						query_keyword(n->content_) == KEYWORD_ELSE) {
					return true;
				}
				return false;
			};
			bool repair_token = false;
			ast_node_t* true_statements = create_ast_node(NODE_BLOCK_STATEMENTS);
			ast_node_t* false_statements = nullptr;
			if (next->tag_ == TOKEN_LBRACE) {
				for (;;) {
					const auto pp = prev_token(c);
					if (pp->tag_ == TOKEN_RBRACE) {
						// RBRACEは食われてた
						break;
					}
					const auto statement = parse_statement(c);
					if (statement == nullptr) {
						raise_error("if contains statements that cannot be parsed.@@ %d to %d Row", pp->appear_line_, ident->appear_line_);
					}
					true_statements =
						create_ast_node(NODE_BLOCK_STATEMENTS, true_statements, statement);
				}
			} else {
				unread_token(c);
				{
					const auto nn = read_token(c);
					if (nn->tag_ != TOKEN_EOS) {
						raise_error("After if conditional, only { or : @@ %d Row", nn->appear_line_);
					}
				}
				for (;;) {
					const auto pp = prev_token(c);
					if (pp->tag_ == TOKEN_EOL) {
						// EOLは食われてた
						repair_token = true;
						break;
					}
					const auto nn = read_token(c);
					unread_token(c);
					if (is_else_token(nn)) {
						break;
					}
					const auto statement = parse_statement(c);
					if (statement == nullptr) {
						raise_error("if contains statements that cannot be parsed.@@ %d to %d Row", nn->appear_line_, ident->appear_line_);
					}
					true_statements =
						create_ast_node(NODE_BLOCK_STATEMENTS, true_statements, statement);
				}
			}
			// elseはあるか？
			const auto nn = read_token(c);
			if (is_else_token(nn)) {
				repair_token = false;
				false_statements = create_ast_node(NODE_BLOCK_STATEMENTS);
				const auto nextf = read_token(c);
				if (nextf->tag_ == TOKEN_LBRACE) {
					for (;;) {
						const auto ppf = prev_token(c);
						if (ppf->tag_ == TOKEN_RBRACE) {
							// RBRACEは食われてた
							break;
						}
						const auto statement = parse_statement(c);
						if (statement == nullptr) {
							raise_error("else contains statements that cannot be parsed.@@ %d to %d Row", nn->appear_line_, ident->appear_line_);
						}
						false_statements = create_ast_node(NODE_BLOCK_STATEMENTS, false_statements, statement);
					}
				} else {
					unread_token(c);
					{
						const auto nnf = read_token(c);
						if (nnf->tag_ != TOKEN_EOS) {
							raise_error("After else, only { or : @@ %d Row", nnf->appear_line_);
						}
					}
					for (;;) {
						const auto pp = prev_token(c);
						if (pp->tag_ == TOKEN_EOL) {
							// EOLは食われてた
							repair_token = true;
							break;
						}
						const auto nnf = read_token(c);
						unread_token(c);
						if (is_else_token(nnf)) {
							break;
						}
						const auto statement = parse_statement(c);
						if (statement == nullptr) {
							raise_error("else contains statements that cannot be parsed.@@ %d to %d Row", nnf->appear_line_, ident->appear_line_);
						}
						false_statements = create_ast_node(NODE_BLOCK_STATEMENTS, false_statements, statement);
					}
				}
			} else {
				unread_token(c);
			}
			// EOLを食いすぎてしまった場合用
			if (repair_token) {
				unread_token(c);
			}
			ast_node_t* dispatcher =
				create_ast_node(NODE_IF_DISPATCHER, true_statements, false_statements);
			return create_ast_node(NODE_IF, expr, dispatcher);
		}
		case KEYWORD_ELSE:
			raise_error("Detected an unprocessed else.@@ %d Row", ident->appear_line_);
			break;
	}
	unread_token(c);
	return nullptr;
}

ast_node_t*
parse_command_safe(parse_context_t& c)
{
	const auto ident = read_token(c);
	if (ident->tag_ != TOKEN_IDENTIFIER) {
		unread_token(c);
		return nullptr;
	}
	const auto next = read_token(c);
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
		return nullptr;
	}
	// あるなら引数の解析
	ast_node_t* args = nullptr;
	if (!is_eos_like_token(next->tag_)) {
		unread_token(c);
		args = parse_arguments(c);
	} else {
		unread_token(c);
	}
	const auto command = create_ast_node(NODE_COMMAND, ident, args);
	return command;
}

ast_node_t*
parse_arguments(parse_context_t& c)
{
	auto arg = parse_expression(c);
	auto res = create_ast_node(NODE_ARGUMENTS, arg);
	auto args = res;
	for (;;) {
		const auto token = read_token(c);
		if (token->tag_ != TOKEN_COMMA) {
			unread_token(c);
			break;
		}
		auto next = parse_expression(c);
		args->right_ = create_ast_node(NODE_ARGUMENTS, next);
		args = args->right_;
	}
	return res;
}

ast_node_t*
parse_assign_safe(parse_context_t& c)
{
	auto variable = parse_variable_safe(c);
	if (variable == nullptr) {
		return nullptr;
	}
	const auto next = read_token(c);
	if (next->tag_ != TOKEN_ASSIGN) {
		raise_error("Value substitution: = is required.@@ %d Row", next->appear_line_);
	}
	auto expr = parse_expression(c);
	auto assign = create_ast_node(NODE_ASSIGN, variable, expr);
	return assign;
}

ast_node_t*
parse_variable_safe(parse_context_t& c)
{
	const auto ident = read_token(c);
	if (ident->tag_ != TOKEN_IDENTIFIER) {
		unread_token(c);
		return nullptr;
	}
	const auto next = read_token(c);
	if (next->tag_ != TOKEN_LPARENTHESIS) {
		unread_token(c);
		return create_ast_node(NODE_VARIABLE, ident);
	}
	const auto idx = parse_expression(c);
	const auto nn = read_token(c);
	if (nn->tag_ != TOKEN_RPARENTHESIS) {
		// 多そうなので個別対処
		if (nn->tag_ == TOKEN_COMMA) {
			raise_error("Array variable is one dimension only.@@ %d Row", nn->appear_line_);
		}
		raise_error("Array variable: Parentheses are not closed.@@ %d Row", nn->appear_line_);
	}
	return create_ast_node(NODE_VARIABLE, ident, idx);
}

ast_node_t*
parse_expression(parse_context_t& c)
{
	// ただの関数転送
	return parse_borand(c);
}

ast_node_t*
parse_borand(parse_context_t& c)
{
	auto node = parse_eqneq(c);
	for (;;) {
		bool is_continue = true;
		const auto token = read_token(c);
		switch (token->tag_) {
			case TOKEN_OP_BOR:
			case TOKEN_OP_BAND: {
				auto r = parse_eqneq(c);
				if (r == nullptr) {
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
			unread_token(c);
			break;
		}
	}
	return node;
}

ast_node_t*
parse_eqneq(parse_context_t& c)
{
	auto node = parse_gtlt(c);
	for (;;) {
		bool is_continue = true;
		const auto token = read_token(c);
		switch (token->tag_) {
			case TOKEN_OP_EQ:
			case TOKEN_OP_NEQ:
			case TOKEN_ASSIGN: {
				auto r = parse_gtlt(c);
				if (r == nullptr) {
					raise_error("==,!=  Operator analysis failed.@@ %d Row", token->appear_line_);
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
			unread_token(c);
			break;
		}
	}
	return node;
}

ast_node_t*
parse_gtlt(parse_context_t& c)
{
	auto node = parse_addsub(c);
	for (;;) {
		bool is_continue = true;
		const auto token = read_token(c);
		switch (token->tag_) {
			case TOKEN_OP_GT:
			case TOKEN_OP_GTOE:
			case TOKEN_OP_LT:
			case TOKEN_OP_LTOE: {
				auto r = parse_addsub(c);
				if (r == nullptr) {
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
			unread_token(c);
			break;
		}
	}
	return node;
}

ast_node_t*
parse_addsub(parse_context_t& c)
{
	auto node = parse_muldivmod(c);
	for (;;) {
		bool is_continue = true;
		const auto token = read_token(c);
		switch (token->tag_) {
			case TOKEN_OP_ADD:
			case TOKEN_OP_SUB: {
				auto r = parse_muldivmod(c);
				if (r == nullptr) {
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
			unread_token(c);
			break;
		}
	}
	return node;
}

ast_node_t*
parse_muldivmod(parse_context_t& c)
{
	auto node = parse_term(c);
	for (;;) {
		bool is_continue = true;
		const auto token = read_token(c);
		switch (token->tag_) {
			case TOKEN_OP_MUL:
			case TOKEN_OP_DIV:
			case TOKEN_OP_MOD: {
				auto r = parse_term(c);
				if (r == nullptr) {
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
			unread_token(c);
			break;
		}
	}
	return node;
}

ast_node_t*
parse_term(parse_context_t& c)
{
	const auto token = read_token(c);
	switch (token->tag_) {
		case TOKEN_OP_SUB:
			return create_ast_node(NODE_UNARY_MINUS, parse_primitive(c));
		default:
			break;
	}
	unread_token(c);
	return parse_primitive(c);
}

ast_node_t*
parse_primitive(parse_context_t& c)
{
	const auto token = read_token(c);
	switch (token->tag_) {
		case TOKEN_LPARENTHESIS: {
			const auto node = parse_expression(c);
			const auto next = read_token(c);
			if (next->tag_ != TOKEN_RPARENTHESIS) {
				raise_error("Parentheses are not closed.@@ %d Row", token->appear_line_);
			}
			return create_ast_node(NODE_EXPRESSION, node);
		}
		case TOKEN_INTEGER:
		case TOKEN_REAL:
		case TOKEN_STRING:
			return create_ast_node(NODE_PRIMITIVE_VALUE, token);
		case TOKEN_OP_MUL: {
			unread_token(c);
			const auto label = parse_label_safe(c);
			if (label == nullptr) {
				raise_error("Label cannot be parsed.@@ %d Row", token->appear_line_);
			}
			raise_error("Labels cannot be used for expressions.@@ %d Row", token->appear_line_);
			return label;
		}
		case TOKEN_IDENTIFIER: {
			unread_token(c);
			const auto expr = parse_identifier_expression(c);
			if (expr == nullptr) {
				raise_error("Function or Variable cannot be parsed.@@ %d Row", token->appear_line_);
			}
			return expr;
		}
		default:
			break;
	}
	raise_error("Primitive values cannot be acquired.@@ %d Row[%s]", token->appear_line_, token->content_);
	return nullptr;
}

ast_node_t*
parse_identifier_expression(parse_context_t& c)
{
	const auto ident = read_token(c);
	if (ident->tag_ != TOKEN_IDENTIFIER) {
		unread_token(c);
		return nullptr;
	}
	const auto next = read_token(c);
	if (next->tag_ != TOKEN_LPARENTHESIS) {
		unread_token(c);
		return create_ast_node(NODE_IDENTIFIER_EXPR, ident);
	}
	// 引数なしの即閉じ？
	{
		const auto nn = read_token(c);
		if (nn->tag_ == TOKEN_RPARENTHESIS) {
			return create_ast_node(NODE_IDENTIFIER_EXPR, ident, create_ast_node(NODE_ARGUMENTS));
		}
		unread_token(c);
	}
	// 引数あり
	const auto idx = parse_arguments(c);
	const auto nn = read_token(c);
	if (nn->tag_ != TOKEN_RPARENTHESIS) {
		raise_error("Function or Array variable: Parentheses are not closed.@@ %d Row", nn->appear_line_);
	}
	return create_ast_node(NODE_IDENTIFIER_EXPR, ident, idx);
}

// 変数
variable_t*
create_variable(const char* name)
{
	const auto res = reinterpret_cast<variable_t*>(xmalloc(sizeof(variable_t)));
	res->name_ = create_string(name);
	res->type_ = VALUE_NONE;
	res->granule_size_ = 0;
	res->length_ = 0;
	res->data_ = nullptr;
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
	if (v->data_ != nullptr) {
		xfree(v->data_);
		v->data_ = nullptr;
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
	v->data_ = xmalloc(areasize);
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
	auto node = table->head_;
	while (node != nullptr) {
		const auto var = reinterpret_cast<variable_t*>(node->value_);
		destroy_variable(var);
		node = node->next_;
	}
	list_free_all(*table);
	destroy_list(table);
}

variable_t*
search_variable(list_t* table, const char* name)
{
	auto node = table->head_;
	while (node != nullptr) {
		const auto var = reinterpret_cast<variable_t*>(node->value_);
		if (string_equal_igcase(var->name_, name)) {
			return var;
		}
		node = node->next_;
	}
	return nullptr;
}

void
variable_set(list_t* table, const value_t& v, const char* name, int idx)
{
	auto var = search_variable(table, name);
	if (var == nullptr) {
		var = create_variable(name);
		auto node = create_list_node();
		node->value_ = var;
		list_append(*table, node);
	}
	assert(var != nullptr);
	if (var->type_ != v.type_) {
		if (idx > 0) {
			raise_error("Variable type is different.@@ %s(%d)", name, idx);
		}
		prepare_variable(var, v.type_, 64, 16);
	}
	bool init_required = false;
	int granule_size = 0;
	if (v.type_ == VALUE_STRING) {
		granule_size = strlen(v.svalue_) + 1;
	}
	if (var->granule_size_ < granule_size) {
		init_required = true;
	}
	int len = var->length_;
	if (idx < 0 || len <= idx) {
		raise_error("Invalid value.@@ %s(%d)", name, idx);
	}
	if (init_required) {
		prepare_variable(var, v.type_, granule_size, len);
	}
	assert(var->type_ == v.type_);
	auto data_ptr = variable_data_ptr(*var, idx);
	switch (var->type_) {
		case VALUE_INT: {
			reinterpret_cast<int*>(data_ptr)[0] = v.ivalue_;
			break;
		}
		case VALUE_DOUBLE: {
			reinterpret_cast<double*>(data_ptr)[0] = v.dvalue_;
			break;
		}
		case VALUE_STRING: {
			strcpy(reinterpret_cast<char*>(data_ptr), v.svalue_);
			break;
		}
		default:
			assert(false);
			break;
	}
}

void*
variable_data_ptr(const variable_t& v, int idx)
{
	if (idx < 0 || idx >= v.length_) {
		raise_error("Variable is out of range.@@ %s(%d)", v.name_, idx);
	}
	switch (v.type_) {
		case VALUE_INT: {
			return reinterpret_cast<int*>(v.data_) + idx;
		}
		case VALUE_DOUBLE: {
			return reinterpret_cast<double*>(v.data_) + idx;
		}
		case VALUE_STRING: {
			return reinterpret_cast<char*>(v.data_) + v.granule_size_ * idx;
		}
		default:
			assert(false);
			break;
	}
	return nullptr;
}

int
variable_calc_int(const variable_t& r, int idx)
{
	const auto data_ptr = variable_data_ptr(r, idx);
	switch (r.type_) {
		case VALUE_INT:
			return *reinterpret_cast<int*>(data_ptr);
		case VALUE_DOUBLE:
			return static_cast<int>(*reinterpret_cast<double*>(data_ptr));
		case VALUE_STRING:
			return atoi(reinterpret_cast<const char*>(data_ptr));
		default:
			assert(false);
			break;
	}
	return 0;
}

double
variable_calc_double(const variable_t& r, int idx)
{
	const auto data_ptr = variable_data_ptr(r, idx);
	switch (r.type_) {
		case VALUE_INT:
			return static_cast<double>(*reinterpret_cast<int*>(data_ptr));
		case VALUE_DOUBLE:
			return *reinterpret_cast<double*>(data_ptr);
		case VALUE_STRING:
			return atof(reinterpret_cast<const char*>(data_ptr));
		default:
			assert(false);
			break;
	}
	return 0.0;
}

char*
variable_calc_string(const variable_t& r, int idx)
{
	const auto data_ptr = variable_data_ptr(r, idx);
	switch (r.type_) {
		case VALUE_INT:
			return create_string_from(*reinterpret_cast<int*>(data_ptr));
		case VALUE_DOUBLE:
			return create_string_from(*reinterpret_cast<double*>(data_ptr));
		case VALUE_STRING:
			return create_string(reinterpret_cast<const char*>(data_ptr));
		default:
			assert(false);
			break;
	}
	return create_string("");
}

// 値
static value_t*
alloc_value()
{
	return reinterpret_cast<value_t*>(xmalloc(sizeof(value_t)));
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
create_value(double v)
{
	value_t* res = alloc_value();
	res->type_ = VALUE_DOUBLE;
	res->dvalue_ = v;
	return res;
}

value_t*
create_value(const char* v)
{
	value_t* res = alloc_value();
	res->type_ = VALUE_STRING;
	res->svalue_ = create_string(v);
	return res;
}

value_t*
create_value(variable_t* v, int idx)
{
	value_t* res = alloc_value();
	res->type_ = VALUE_VARIABLE;
	res->variable_ = v;
	res->index_ = idx;
	return res;
}

value_t*
create_value(const value_t& v)
{
	value_t* res = alloc_value();
	res->type_ = v.type_;
	switch (v.type_) {
		case VALUE_INT:
			res->ivalue_ = v.ivalue_;
			break;
		case VALUE_DOUBLE:
			res->dvalue_ = v.dvalue_;
			break;
		case VALUE_STRING:
			res->svalue_ = create_string(v.svalue_);
			break;
		case VALUE_VARIABLE:
			res->variable_ = v.variable_;
			res->index_ = v.index_;
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
value_set(value_t* v, double d)
{
	clear_value(v);
	v->type_ = VALUE_DOUBLE;
	v->dvalue_ = d;
}

void
value_set(value_t* v, const char* s)
{
	clear_value(v);
	v->type_ = VALUE_STRING;
	v->svalue_ = create_string(s);
}

void
value_move(value_t* v, char* s)
{
	clear_value(v);
	v->type_ = VALUE_STRING;
	v->svalue_ = s;
	;
}

void
value_move(value_t* to, value_t* from)
{
	clear_value(to);
	to->type_ = from->type_;
	to->value_ = from->value_;
	from->type_ = VALUE_NONE;
	from->value_ = 0;
}

value_tag
value_get_primitive_tag(const value_t& r)
{
	if (r.type_ == VALUE_VARIABLE) {
		return r.variable_->type_;
	}
	return r.type_;
}

bool
value_calc_boolean(const value_t& r)
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
value_calc_int(const value_t& r)
{
	switch (r.type_) {
		case VALUE_INT:
			return r.ivalue_;
		case VALUE_DOUBLE:
			return static_cast<int>(r.dvalue_);
		case VALUE_STRING:
			return atoi(r.svalue_);
		case VALUE_VARIABLE:
			return variable_calc_int(*r.variable_, r.index_);
			break;
		default:
			assert(false);
			break;
	}
	return 0;
}

double
value_calc_double(const value_t& r)
{
	switch (r.type_) {
		case VALUE_INT:
			return static_cast<double>(r.ivalue_);
		case VALUE_DOUBLE:
			return r.dvalue_;
		case VALUE_STRING:
			return atof(r.svalue_);
		case VALUE_VARIABLE:
			return variable_calc_double(*r.variable_, r.index_);
			break;
		default:
			assert(false);
			break;
	}
	return 0.0;
}

char*
value_calc_string(const value_t& r)
{
	char* s = nullptr;
	switch (r.type_) {
		case VALUE_INT:
			s = create_string_from(r.ivalue_);
			break;
		case VALUE_DOUBLE:
			s = create_string_from(r.dvalue_);
			break;
		case VALUE_STRING:
			s = create_string(r.svalue_);
			break;
		case VALUE_VARIABLE:
			s = variable_calc_string(*r.variable_, r.index_);
			break;
		default:
			assert(false);
			break;
	}
	return s;
}

value_t*
value_convert_type(value_tag to, const value_t& r)
{
	if (to == r.type_) {
		return create_value(r);
	}
	switch (to) {
		case VALUE_INT:
			return create_value(value_calc_int(r));
		case VALUE_DOUBLE:
			return create_value(value_calc_double(r));
		case VALUE_STRING: {
			auto s = value_calc_string(r);
			auto res = create_value_move(s);
			return res;
		}
		default:
			assert(false);
			break;
	}
	return nullptr;
}

void
value_isolate(value_t& v)
{
	if (v.type_ != VALUE_VARIABLE) {
		return;
	}
	switch (v.variable_->type_) {
		case VALUE_INT:
			value_set(&v, variable_calc_int(*v.variable_, v.index_));
			break;
		case VALUE_DOUBLE:
			value_set(&v, variable_calc_double(*v.variable_, v.index_));
			break;
		case VALUE_STRING:
			value_move(&v, variable_calc_string(*v.variable_, v.index_));
			break;
		default:
			assert(false);
			break;
	}
}

void
value_bor(value_t* v, const value_t& r)
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
value_band(value_t* v, const value_t& r)
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
value_eq(value_t* v, const value_t& r)
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
value_neq(value_t* v, const value_t& r)
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
value_gt(value_t* v, const value_t& r)
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
value_gtoe(value_t* v, const value_t& r)
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
value_lt(value_t* v, const value_t& r)
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
value_ltoe(value_t* v, const value_t& r)
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
value_add(value_t* v, const value_t& r)
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
			const auto required =
				snprintf(nullptr, 0, "%s%s", v->svalue_, t->svalue_);
			auto s = create_string(required);
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
value_sub(value_t* v, const value_t& r)
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
value_mul(value_t* v, const value_t& r)
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
value_div(value_t* v, const value_t& r)
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
value_mod(value_t* v, const value_t& r)
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
	auto res = reinterpret_cast<value_stack_t*>(xmalloc(sizeof(value_stack_t)));
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
	const auto l = 16; // 初期サイズ
	st->stack_ = reinterpret_cast<value_t**>(xmalloc(sizeof(value_t*) * l));
	st->top_ = 0;
	st->max_ = l;
}

void
uninitialize_value_stack(value_stack_t* st)
{
	stack_pop(st, st->top_);
	xfree(st->stack_);
	st->stack_ = nullptr;
	st->top_ = 0;
	st->max_ = 0;
}

void
stack_push(value_stack_t* st, value_t* v)
{
	if (st->top_ + 1 > st->max_) {
		st->max_ = st->max_ * 2; // 貪欲
		st->stack_ = reinterpret_cast<value_t**>(xrealloc(st->stack_, sizeof(value_t*) * st->max_));
	}
	st->stack_[st->top_] = v;
	++st->top_;
}

void
stack_push(value_stack_t* st, const value_t& v)
{
	value_t* vp = create_value(v);
	stack_push(st, vp);
}

value_t*
stack_peek(value_stack_t* st, int i)
{
	const auto idx = (i < 0 ? st->top_ + i : i);
	assert(idx >= 0 && idx < st->top_);
	return st->stack_[idx];
}

void
stack_pop(value_stack_t* st, size_t n)
{
	assert(n <= static_cast<size_t>(st->top_));
	while (n-- > 0) {
		--st->top_;
		assert(st->stack_[st->top_] != nullptr);
		destroy_value(st->stack_[st->top_]);
		st->stack_[st->top_] = nullptr;
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
		{ -1, nullptr },
	};
	// 全探索
	for (int i = 0; table[i].tag_ != -1; ++i) {
		if (string_equal_igcase(s, table[i].word_)) {
			return table[i].tag_;
		}
	}
	return -1;
}

// 実行環境
execute_environment_t*
create_execute_environment()
{
	auto res = reinterpret_cast<execute_environment_t*>(xmalloc(sizeof(execute_environment_t)));
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
		auto node = e->statement_list_->head_;
		while (node != nullptr) {
			const auto ast = reinterpret_cast<ast_node_t*>(node->value_);
			if (ast->flag_ & NODE_FLAG_ADDITIONAL) {
				destroy_ast_node(ast);
			}
			node = node->next_;
		}
		list_free_all(*e->statement_list_);
		destroy_list(e->statement_list_);
	}
	{
		auto node = e->parser_list_->head_;
		while (node != nullptr) {
			const auto parser = reinterpret_cast<parse_context_t*>(node->value_);
			uninitialize_parse_context(parser);
			destroy_parse_context(parser);
			node = node->next_;
		}
		list_free_all(*e->parser_list_);
		destroy_list(e->parser_list_);
	}
	{
		auto node = e->ast_list_->head_;
		while (node != nullptr) {
			const auto ast = reinterpret_cast<list_t*>(node->value_);
			destroy_ast(ast);
			node = node->next_;
		}
		list_free_all(*e->ast_list_);
		destroy_list(e->ast_list_);
	}
	{
		auto node = e->label_table_->head_;
		while (node != nullptr) {
			const auto label_node = reinterpret_cast<label_node_t*>(node->value_);
			xfree(label_node->name_);
			xfree(label_node);
			node->value_ = nullptr;
			node = node->next_;
		}
		list_free_all(*e->label_table_);
		destroy_list(e->label_table_);
	}
	destroy_variable_table(e->variable_table_);
	xfree(e);
}

void
initialize_execute_status(execute_status_t* s)
{
	s->stack_ = create_value_stack();
	s->node_cur_ = nullptr;
	s->current_call_frame_ = 0;
	s->call_frame_[0].caller_ = nullptr;
	s->current_loop_frame_ = 0;
	s->loop_frame_[0].counter_ = 0;
	s->loop_frame_[0].max_ = 0;
	s->loop_frame_[0].cnt_ = 0;
	s->is_end_ = false;
	s->stat_ = 0;
	s->refdval_ = 0.0;
	s->refstr_ = create_string("");
	s->strsize_ = 0;
}

void
uninitialize_execute_status(execute_status_t* s)
{
	destroy_value_stack(s->stack_);
	destroy_string(s->refstr_);
	s->refstr_ = nullptr;
}

void
load_script(execute_environment_t* e, const char* script)
{
	tokenize_context_t tokenizer;
	initialize_tokenize_context(&tokenizer, script);
	auto parser = create_parse_context();
	initialize_parse_context(parser, tokenizer);
	auto ast = parse_script(*parser);
	uninitialize_tokenize_context(&tokenizer);
	// ASTを繋げたりラベルを持っておいたり
	{
		list_node_t* st = ast->head_;
		while (st != nullptr) {
			ast_node_t* node = reinterpret_cast<ast_node_t*>(st->value_);
			struct _
			{
				static void flatten(execute_environment_t* e, ast_node_t* node)
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
					} else if (node->tag_ == NODE_IF) {
						// 処理用のノードを付け足す
						{
							auto check_node = create_list_node();
							const auto checker = create_ast_node(NODE_IF_CHECK);
							checker->flag_ |= NODE_FLAG_ADDITIONAL;
							checker->ext_ = node->left_;
							check_node->value_ = checker;
							list_append(*e->statement_list_, check_node);
						}
						// 各ブロックを線形に貼りなおす
						{
							const auto dispatcher = node->right_;
							assert(dispatcher->tag_ == NODE_IF_DISPATCHER);
							const auto cl = [&]() {
								const auto n = create_ast_node(NODE_JUMP_LABEL);
								n->flag_ |= NODE_FLAG_ADDITIONAL;
								return n;
							};
							const auto cj = [&]() {
								const auto n = create_ast_node(NODE_JUMP_INTERNAL);
								n->flag_ |= NODE_FLAG_ADDITIONAL;
								return n;
							};
							const auto aj = [&](list_node_t* tail) {
								const auto j = cj();
								j->ext_ = tail;
								const auto jumper = create_list_node();
								jumper->value_ = j;
								list_append(*e->statement_list_, jumper);
							};
							const auto true_head = create_list_node();
							true_head->value_ = cl();
							const auto false_head = create_list_node();
							false_head->value_ = cl();
							const auto tail = create_list_node();
							tail->value_ = cl();
							// 分岐
							aj(true_head);
							aj(false_head);
							// 真
							list_append(*e->statement_list_, true_head);
							flatten(e, dispatcher->left_);
							aj(tail);
							// 偽
							list_append(*e->statement_list_, false_head);
							if (dispatcher->right_) {
								flatten(e, dispatcher->right_);
								aj(tail);
							}
							// 合流
							list_append(*e->statement_list_, tail);
						}
						is_add = false;
					}
					// 後処理
					if (is_add) {
						auto list_node = create_list_node();
						list_node->value_ = node;
						list_append(*e->statement_list_, list_node);
						if (node->tag_ == NODE_LABEL) {
							auto label_node = create_list_node();
							label_node_t* label =
								reinterpret_cast<label_node_t*>(xmalloc(sizeof(label_node_t)));
							label->name_ = create_string(node->token_->content_);
							label->statement_ = list_node;
							label_node->value_ = label;
							list_append(*e->label_table_, label_node);
						} else if (node->tag_ == NODE_REPEAT) {
							// 処理用のノードを付け足す
							auto check_node = create_list_node();
							const auto checker = create_ast_node(NODE_REPEAT_CHECK);
							checker->flag_ |= NODE_FLAG_ADDITIONAL;
							check_node->value_ = checker;
							list_append(*e->statement_list_, check_node);
						}
					}
				}
			};
			_::flatten(e, node);
			st = st->next_;
		}
	}
	// 特定の部分木マッチング
	{
		struct _
		{
			static void walk(execute_environment_t* e, ast_node_t* node)
			{
				if (node->tag_ == NODE_VARIABLE ||
						node->tag_ == NODE_IDENTIFIER_EXPR /*変数配列の可能性あり*/) {
					auto var_name = node->token_->content_;
					if (search_variable(e->variable_table_, var_name) == nullptr) {
						// 適当な変数として初期化しておく
						value_t v;
						v.type_ = VALUE_INT;
						v.ivalue_ = 0;
						variable_set(e->variable_table_, v, var_name, 0);
					}
				}
				if (node->left_ != nullptr) {
					walk(e, node->left_);
				}
				if (node->right_ != nullptr) {
					walk(e, node->right_);
				}
			}
		};
		list_node_t* st = ast->head_;
		while (st != nullptr) {
			ast_node_t* node = reinterpret_cast<ast_node_t*>(st->value_);
			_::walk(e, node);
			st = st->next_;
		}
	}
	// パーサーとASTを保存しておく
	{
		auto parser_node = create_list_node();
		parser_node->value_ = parser;
		list_append(*e->parser_list_, parser_node);
	}
	{
		auto ast_node = create_list_node();
		ast_node->value_ = ast;
		list_append(*e->ast_list_, ast_node);
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
			const auto command_name = n->token_->content_;
			const auto command = query_command(command_name);
			if (command == -1) {
				raise_error("Command not found:%s", command_name);
			}
			const auto delegate = get_command_delegate(static_cast<builtin_command_tag>(command));
			assert(delegate != nullptr);
			const auto top = s->stack_->top_;
			if (n->left_ != nullptr) {
				evaluate(e, s, n->left_);
			}
			const auto arg_num = s->stack_->top_ - top;
			delegate(e, s, arg_num);
			assert(s->stack_->top_ == top); // 戻り値がないことを確認
			break;
		}
		case NODE_ARGUMENTS: {
			if (n->left_ != nullptr) {
				evaluate(e, s, n->left_);
			}
			if (n->right_ != nullptr) {
				evaluate(e, s, n->right_);
			}
			break;
		}
		case NODE_ASSIGN: {
			evaluate(e, s, n->left_);
			evaluate(e, s, n->right_);
			const auto var = stack_peek(s->stack_, -2);
			if (var->type_ != VALUE_VARIABLE) {
				raise_error("Substitution: Please specify a variable.");
			}
			const auto v = stack_peek(s->stack_, -1);
			value_isolate(*v);
			variable_set(e->variable_table_, *v, var->variable_->name_, var->index_);
			stack_pop(s->stack_, 2);
			break;
		}
		case NODE_VARIABLE: {
			const auto var_name = n->token_->content_;
			const auto var = search_variable(e->variable_table_, var_name);
			assert(var != nullptr);
			int idx = 0;
			auto idx_node = n->left_;
			if (idx_node) {
				evaluate(e, s, idx_node);
				auto i = stack_peek(s->stack_);
				idx = value_calc_int(*i);
				stack_pop(s->stack_);
			}
			value_t* v = create_value(var, idx);
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
			value_isolate(*l);
			switch (n->tag_) {
				case NODE_BOR:
					value_bor(l, *r);
					break;
				case NODE_BAND:
					value_band(l, *r);
					break;
				case NODE_EQ:
					value_eq(l, *r);
					break;
				case NODE_NEQ:
					value_neq(l, *r);
					break;
				case NODE_GT:
					value_gt(l, *r);
					break;
				case NODE_GTOE:
					value_gtoe(l, *r);
					break;
				case NODE_LT:
					value_lt(l, *r);
					break;
				case NODE_LTOE:
					value_ltoe(l, *r);
					break;
				case NODE_ADD:
					value_add(l, *r);
					break;
				case NODE_SUB:
					value_sub(l, *r);
					break;
				case NODE_MUL:
					value_mul(l, *r);
					break;
				case NODE_DIV:
					value_div(l, *r);
					break;
				case NODE_MOD:
					value_mod(l, *r);
					break;
				default:
					assert(false);
					break;
			}
			stack_pop(s->stack_);
			break;
		}
		case NODE_UNARY_MINUS: {
			assert(n->left_ != nullptr);
			evaluate(e, s, n->left_);
			value_t* v = stack_peek(s->stack_);
			value_isolate(*v);
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
					stack_push(s->stack_, create_value(atof(n->token_->content_)));
					break;
				case TOKEN_STRING:
					stack_push(s->stack_, create_value(n->token_->content_));
					break;
				default:
					assert(false);
					break;
			}
			break;
		}
		case NODE_IDENTIFIER_EXPR: {
			assert(n->token_->tag_ == TOKEN_IDENTIFIER);
			const auto ident = n->token_->content_;
			const auto top = s->stack_->top_;
			if (n->left_ != nullptr) {
				evaluate(e, s, n->left_);
			}
			const auto arg_num = s->stack_->top_ - top;
			const auto function = query_function(ident);
			if (function >= 0) {
				// 関数呼び出し
				const auto delegate =
					get_function_delegate(static_cast<builtin_function_tag>(function));
				assert(delegate != nullptr);
				delegate(e, s, arg_num);
				assert(s->stack_->top_ == top + 1); // 戻り値が入っていることを確認する
			} else {
				// システム変数
				const auto sysvar = query_sysvar(ident);
				if (sysvar >= 0) {
					if (arg_num > 0) {
						raise_error("cannot arrays in the system variable.", ident);
					}
					// 後々のことも考えて、一応
					stack_pop(s->stack_, arg_num);
					switch (sysvar) {
						case SYSVAR_CNT:
							if (s->current_loop_frame_ <= 0) {
								raise_error(
									"System variable(cnt): cannot refer outside the repeat-loop.");
							}
							stack_push(s->stack_, create_value(s->loop_frame_[s->current_loop_frame_ - 1].cnt_));
							break;
						case SYSVAR_STAT:
							stack_push(s->stack_, create_value(s->stat_));
							break;
						case SYSVAR_REFDVAL:
							stack_push(s->stack_, create_value(s->refdval_));
							break;
						case SYSVAR_REFSTR:
							stack_push(s->stack_, create_value(s->refstr_));
							break;
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
						default:
							assert(false);
							stack_push(s->stack_, create_value(0));
							break;
					}
				} else {
					// 配列変数
					if (arg_num > 1) {
						raise_error("Function not found, Array variable is one dimension only.@@ %s", ident);
					}
					const auto var = search_variable(e->variable_table_, ident);
					assert(var != nullptr);
					int idx = 0;
					if (arg_num > 0) {
						auto i = stack_peek(s->stack_);
						idx = value_calc_int(*i);
					}
					value_t* v = create_value(var, idx);
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
				const auto res = stack_peek(s->stack_);
				switch (value_get_primitive_tag(*res)) {
					case VALUE_INT:
						s->stat_ = value_calc_int(*res);
						break;
					case VALUE_DOUBLE:
						s->refdval_ = value_calc_double(*res);
						break;
					case VALUE_STRING:
						destroy_string(s->refstr_);
						s->refstr_ = value_calc_string(*res);
						break;
					default:
						assert(false);
						break;
				}
				stack_pop(s->stack_);
			}
			--s->current_call_frame_;
			const auto& frame = s->call_frame_[s->current_call_frame_];
			s->node_cur_ = frame.caller_;
			break;
		}
		case NODE_GOTO: {
			const auto label_node = n->left_;
			assert(label_node != nullptr);
			assert(label_node->tag_ == NODE_LABEL);
			const auto label_name = label_node->token_->content_;
			const auto label = search_label(e, label_name);
			if (label == nullptr) {
				raise_error("goto: Label not found.@@ %s", label_name);
			}
			s->node_cur_ = label;
			break;
		}
		case NODE_GOSUB: {
			const auto label_node = n->left_;
			assert(label_node != nullptr);
			assert(label_node->tag_ == NODE_LABEL);
			const auto label_name = label_node->token_->content_;
			const auto label = search_label(e, label_name);
			if (label == nullptr) {
				raise_error("gosub: Label not found.@@ %s", label_name);
			}
			if ((s->current_call_frame_ + 1) >= MAX_CALL_FRAME) {
				raise_error("gosub: Nesting is too deep.");
			}
			auto& frame = s->call_frame_[s->current_call_frame_];
			++s->current_call_frame_;
			frame.caller_ = s->node_cur_;
			s->node_cur_ = label;
			break;
		}
		case NODE_REPEAT: {
			if (s->current_loop_frame_ + 1 >= MAX_LOOP_FRAME) {
				raise_error("repeat: Nesting is too deep.");
			}
			auto& frame = s->loop_frame_[s->current_loop_frame_];
			++s->current_loop_frame_;
			frame.start_ = s->node_cur_;
			frame.cnt_ = 0;
			frame.counter_ = 0;
			frame.max_ = 0;
			int loop_num = -1;
			if (n->left_) {
				evaluate(e, s, n->left_);
				const auto v = stack_peek(s->stack_);
				loop_num = value_calc_int(*v);
				stack_pop(s->stack_);
			}
			frame.max_ = loop_num;
			break;
		}
		case NODE_REPEAT_CHECK: {
			assert(s->current_loop_frame_ > 0);
			auto& frame = s->loop_frame_[s->current_loop_frame_ - 1];
			if (frame.max_ >= 0 && frame.counter_ >= frame.max_) {
				int depth = 0;
				while (s->node_cur_ != nullptr) {
					auto ex = reinterpret_cast<ast_node_t*>(s->node_cur_->value_);
					if (ex->tag_ == NODE_REPEAT) {
						++depth;
					} else if (ex->tag_ == NODE_LOOP) {
						if (depth-- == 0) {
							break;
						}
					}
					s->node_cur_ = s->node_cur_->next_;
				}
				assert(s->node_cur_ != nullptr);
				--s->current_loop_frame_;
			}
			break;
		}
		case NODE_LOOP:
		case NODE_CONTINUE: {
			if (s->current_loop_frame_ <= 0) {
				raise_error("loop,continue: is not in repeat-loop.");
			}
			auto& frame = s->loop_frame_[s->current_loop_frame_ - 1];
			++frame.counter_;
			++frame.cnt_;
			s->node_cur_ = frame.start_;
			break;
		}
		case NODE_BREAK: {
			if (s->current_loop_frame_ <= 0) {
				raise_error("break: is not in repeat-loop.");
			}
			int depth = 0;
			while (s->node_cur_ != nullptr) {
				auto ex = reinterpret_cast<ast_node_t*>(s->node_cur_->value_);
				if (ex->tag_ == NODE_REPEAT) {
					++depth;
				} else if (ex->tag_ == NODE_LOOP) {
					if (depth-- == 0) {
						break;
					}
				}
				s->node_cur_ = s->node_cur_->next_;
			}
			if (s->node_cur_ == nullptr) {
				raise_error("break: couldn't repeat-loop in goto, and break.");
			}
			--s->current_loop_frame_;
			break;
		}
		case NODE_IF: {
			assert(n->left_ != nullptr);
			evaluate(e, s, n->left_);
			assert(n->right_ != nullptr);
			const auto dispatcher = n->right_;
			assert(dispatcher->tag_ == NODE_IF_DISPATCHER);
			const auto cond = stack_peek(s->stack_);
			const auto is_cond = value_calc_boolean(*cond);
			stack_pop(s->stack_);
			if (is_cond) {
				evaluate(e, s, dispatcher->left_);
			} else {
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
			assert(n->ext_ != nullptr);
			const auto c = reinterpret_cast<ast_node_t*>(n->ext_);
			evaluate(e, s, c);
			const auto cond = stack_peek(s->stack_);
			const auto is_cond = value_calc_boolean(*cond);
			stack_pop(s->stack_);
			if (!is_cond) {
				s->node_cur_ = s->node_cur_->next_;
			}
			break;
		}
		case NODE_JUMP_LABEL:
			break;
		case NODE_JUMP_INTERNAL:
			assert(n->ext_ != nullptr);
			s->node_cur_ = reinterpret_cast<list_node_t*>(n->ext_);
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
	if (s.node_cur_ == nullptr) {
		raise_error("No executable node.@@ [%p]", e);
	}
	for (;;) {
		auto ex = reinterpret_cast<ast_node_t*>(s.node_cur_->value_);
		const auto top = s.stack_->top_;
		evaluate(e, &s, ex);
		assert(top == s.stack_->top_);
		if (s.is_end_) {
			// もう実行終わったらしい、帰る
			break;
		}
		s.node_cur_ = s.node_cur_->next_;
		if (s.node_cur_ == nullptr) {
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
		{ COMMAND_DEVTERM, "devterm" },
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
			COMMAND_RANDOMIZE, "randomize",
		},
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
		{ -1, nullptr },
	};
	// 全探索
	for (int i = 0; table[i].tag_ != -1; ++i) {
		if (string_equal_igcase(s, table[i].word_)) {
			return table[i].tag_;
		}
	}
	return -1;
}

command_delegate
get_command_delegate(builtin_command_tag command)
{
	static const command_delegate commands[] = {
		&command_devterm,
		&command_dim,
		&command_ddim,
		&command_sdim,
		&command_randomize,
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
		nullptr,
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
		{ -1, nullptr },
	};
	// 全探索
	for (int i = 0; table[i].tag_ != -1; ++i) {
		if (string_equal_igcase(s, table[i].word_)) {
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
		nullptr,
	};
	return functions[function];
}

void
mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
	if (button == 0) { // 左クリック
		current_mouse_down_left = action;
	} else if (button == 1) { // 右クリック
		current_mouse_down_right = action;
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

int
main(int argc, const char* argv[])
{
	// オプション
	const char* filename = "start.hs";
	// オプション解析
	if (argc >= 2) {
		filename = argv[1];
	}
	// システムここから
	initialize_system();
	// ファイル読み込み
	size_t script_size = 0;
	char* script = nullptr;
	{
		FILE* file = fopen(filename, "r");
		if (file == nullptr) {
			printf("ERROR : cannot read such file %s\n", filename);
			return -1;
		}
		fseek(file, 0, SEEK_END);
		const size_t initial_size = ftell(file);
		size_t buffer_size = initial_size + 4; // 初期バッファ
		script = reinterpret_cast<char*>(xmalloc(buffer_size + 1));
		fseek(file, 0, SEEK_SET);
		for (;;) {
			const auto c = fgetc(file);
			if (c == EOF) {
				break;
			}
			const auto ch = static_cast<char>(c);
			if (buffer_size <= script_size) {
				buffer_size *= 2;
				script = reinterpret_cast<char*>(xrealloc(script, buffer_size));
			}
			script[script_size++] = ch;
		}
		script[script_size] = '\0';
		fclose(file);
	}
	assert(script != nullptr);

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
	// 実行
	{
		{
			auto env = create_execute_environment();
			load_script(env, script);
			execute(env);
			destroy_execute_environment(env);
		}
		glfwTerminate();
	}
	xfree(script);
	destroy_system();
	return 0;
}
