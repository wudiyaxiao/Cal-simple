#include "fb3-1.h"
#include "mongoose.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
extern double ans;
struct ast *
newast(int nodetype, struct ast * l, struct ast * r)
{
	struct ast * a = malloc(sizeof(struct ast));

	if(!a)
	{
		yyerror("out of space");
		exit(0);
	}
	a->nodetype = nodetype;
	a->l = l;
	a->r = r;
	return a;
}

struct ast *
newnum(double d)
{
	struct numval *a = malloc(sizeof(struct ast));

	if(!a)
	{
		yyerror("out of space");
		exit(0);
	}
	a->nodetype = 'K';
	a->number = d;
	return (struct ast*)a;
}

double 
eval(struct ast* a)
{
	double v;
	switch(a->nodetype)
	{
		case 'K': v = ((struct numval *)a)->number; break;

		case '+': v = eval(a->l) + eval(a->r); break;
		case '-': v = eval(a->l) - eval(a->r); break;
		case '*': v = eval(a->l) * eval(a->r); break;
		case '/': v = eval(a->l) / eval(a->r); break;
		case '|': v = fabs(eval(a->l)); break;
		case 'M': v = eval(a->l); v = -v;break;
		default: printf("internal error: bad node %c\n", a->nodetype);
	}
	return v;
}

void
treefree(struct ast *a)
{
	if(a->nodetype == 'K')
	{
		free(a);
		return ;
	}
	else if(a->nodetype == '+' || a->nodetype == '-' || a->nodetype == '*' || a->nodetype == '/' || a->nodetype == 'M' || a->nodetype == '|')
	{
		if(a->l != NULL)
		{
			treefree(a->l);
		}
		if(a->r != NULL)
		{
			treefree(a->r);
		}
		free(a);
	}
	else
	{
		printf("internal error: bad node %c\n", a->nodetype);
	}
	return ;
}

void
yyerror(char * s, ...)
{
	va_list ap;
	va_start(ap, s);
	fprintf(stderr, "%d: error: ", yylineno);
	vfprintf(stderr, s, ap);
	fprintf(stderr, "\n");
	return ;
}

void solve(char * s,int len)
{
	char * input = (char*)malloc(sizeof(char)*(len+2));
	for(int i = 0; i < len; ++i)
	{
		input[i]= s[i];
	}
	input[len]='\0';
	input[len+1]='\0';
	yinput(input, len+2);	
	yyparse();
	yfree();
	return ;
}

static int getnum(struct mg_connection *conn)
{
	/**/
	char tp[100];
	for(int i = 0;i < conn->content_len; ++i)
	{
		tp[i] = conn->content[i];
	}
	tp[conn->content_len] = '\0';
	//printf("%s\n", tp);
	solve(conn->content,conn->content_len);
	char * res = (char*)malloc(sizeof(char) * 100);
	//printf("ans: %f\n%", ans);
	sprintf(res, "%4.4g\n", ans);
	mg_send_data(conn,res,strlen(res));
	return 0;
}
 
int
main()
{
	struct mg_server *server;
	server = mg_create_server(NULL);
	mg_set_option(server,"listening_port","8080");
	mg_add_uri_handler(server,"/", getnum);
	for (;;) 
	{
		mg_poll_server(server, 1000);
	}
	mg_destory_server(&server);
	printf("> ");
	return yyparse();
}
