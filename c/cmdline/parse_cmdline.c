/*********************************************************************************
** Copyright(C),2010-2011,teamshare
** FileName: parse_cmdline.c
** Author: liu yixiong 
** Version: v1.0 
** Date: 2014-04-03
** Description: 该文件主要用于生成和解析cmdline，实现方式跟bootloader
	传递cmdline给linux内核的做法一样,cmdline的格式跟bootloader的格式一样.
** Others: 
** Function List: 
** 1.void create_cmdline(const char *buf,char *cmdline);//生成cmdline
** 2.static int parse_cmdline(struct tag *tag); //解析cmdline
** History:  
** 1.Date:
** Author:
** Modification:
** 2.…………
***********************************************************************************/

#include <stdio.h>
#include <string.h>


#if 0
#define DEBUGP printf
#else
#define DEBUGP(fmt, a...)
#endif

#ifndef ENOENT
#define ENOENT  2
#endif

struct lowpower_param;
    
/* Returns 0, or -errno.  arg is in kp->arg. */
typedef int (*param_set_fn)(const char *val, struct lowpower_param *kp);

struct lowpower_param {
	const char *name;
	param_set_fn set;
};  



static inline char dash2underscore(char c)
{
	if (c == '-')
		return '_';
	return c;
}

static inline int parameq(const char *input, const char *paramname)
{
	unsigned int i;
	for (i = 0; dash2underscore(input[i]) == paramname[i]; i++)
		if (input[i] == '\0')
			return 1;
	return 0;
}

static int parse_one(char *param,
		char *val,
		struct lowpower_param *params, 
		unsigned num_params)
{
	unsigned int i;

	/* Find parameter */
	for (i = 0; i < num_params; i++) {
		if (parameq(param, params[i].name)) {
			DEBUGP("They are equal!  Calling %p\n",
					params[i].set);
			return params[i].set(val, &params[i]);
		}
	}

	DEBUGP("Unknown argument `%s'\n", param);
	return -ENOENT;
}


/* You can use " around spaces, but can't escape ". */
/* Hyphens and underscores equivalent in parameter names. */
static char *next_arg(char *args, char **param, char **val)
{
	unsigned int i, equals = 0;
	int in_quote = 0, quoted = 0;
	char *next;

	if (*args == '"') {
		args++;
		in_quote = 1;
		quoted = 1;
	}

	for (i = 0; args[i]; i++) {
		if (args[i] == ' ' && !in_quote)
			break;
		if (equals == 0) {
			if (args[i] == '=')
				equals = i;
		}
		if (args[i] == '"')
			in_quote = !in_quote;
	}

	*param = args;
	if (!equals)
		*val = NULL;
	else {
		args[equals] = '\0';
		*val = args + equals + 1;

		/* Don't include quotes in value. */
		if (**val == '"') {
			(*val)++;
			if (args[i-1] == '"')
				args[i-1] = '\0';
		}
		if (quoted && args[i-1] == '"')
			args[i-1] = '\0';
	}

	if (args[i]) {
		args[i] = '\0';
		next = args + i + 1;
	} else
		next = args + i;

	/* Chew up trailing spaces. */
	while (*next == ' ')
		next++;
	return next;
}

/* Args looks like "foo=bar,bar2 baz=fuz wiz". */
int parse_args(char *args,
		struct lowpower_param *params,
		unsigned num)
{
	char *param, *val;

	printf("Parsing ARGS: %s\n", args);

	/* Chew leading spaces */
	while (*args == ' ')
		args++;

	while (*args) {
		int ret;

		args = next_arg(args, &param, &val);
		ret = parse_one(param, val, params, num);
		switch (ret) {
			case -ENOENT:
				printf("Unknown parameter `%s'\n",param);
				break;
			case 0:
				break;
			default:
				printf("`%s' invalid for parameter `%s'\n",val ?: "", param);
				return ret;
		}
	}
	/* All parsed OK. */
	return 0;
}

unsigned int htoi(const char *str) 
{   
	const char *cp;      
	unsigned int data = 0, bdata = 0;

	for (cp = str, data = 0; *cp != 0; ++cp) 
	{
		if (*cp >= '0' && *cp <= '9')
			bdata = *cp - '0';        
		else if (*cp >= 'A' && *cp <= 'F')
			bdata = *cp - 'A' + 10;   
		else if (*cp >= 'a' && *cp <= 'f')
			bdata = *cp - 'a' + 10;   
		else             
			break;       
		data = (data << 4) | bdata;
	}

	return data;         
}                        

unsigned int atoi(const char *str) 
{
	const char *cp;
	unsigned int data = 0;    

	if (str[0] == '0' && (str[1] == 'X' || str[1] == 'x')) 
		return htoi(str + 2);     

	for (cp = str, data = 0; *cp != 0; ++cp) {
		if (*cp >= '0' && *cp <= '9')
			data = data * 10 + *cp - '0';
		else
			break;
	}

	return data;
}

int parse_keys(const char *val, struct lowpower_param *kp)
{
	int i,j;
	unsigned int keys[20];
	printf("into parse_keys: val=%s \n",val);
	i=0;
	keys[i++]=atoi(val);
	for(;*val!=0;val++)
	{
		if(*val==',' && *(++val)!=0)
		{
			keys[i++]=atoi(val);
		}
	}
	for(j=0;j<i;j++)
	{
		printf("val[%d]=0x%x \n",j,keys[j]);
	}
	return 0;
}
struct lowpower_param g_lowpower_param[]={
	{
		.name="keys",
		.set = parse_keys,
	},
	{
		.name="test",
		.set = 0,
	},
};  





#define tag_size(type)  ((sizeof(struct tag_header) + sizeof(struct type)) >> 2)
#define tag_next(t) ((struct tag *)((char *)(t) + (t)->hdr.size))
struct tag_header { unsigned int size;
	unsigned int tag;
};  
struct tag {
	struct tag_header hdr;
	char    cmdline[1];     /* this is the minimum size */
};

void create_cmdline(const char *buf,char *cmdline)
{
	struct tag *params;
	params = (struct tag*)buf;
	params->hdr.tag = 0x54410003; /* MAGIC */
	params->hdr.size = 256;

	/* cmdline */
	strcpy(params->cmdline,cmdline);
	params = tag_next(params);
	params->hdr.tag = 0x0;
	params->hdr.size = 0;

}

static int parse_cmdline(struct tag *tag)
{
	if (tag->hdr.tag == 0x54410003) 
	{
		parse_args(tag->cmdline,g_lowpower_param,sizeof(g_lowpower_param)/sizeof(struct lowpower_param));
	}
	return 0;
}


    
char cmdline[]=" other=fortest keys=0xaa,0xbb,0xcc,0xaf001,0x0034 name=zhangshan";

int main(int argc, const char *argv[])
{
	char buf[512];
	create_cmdline(buf,cmdline);
	parse_cmdline((struct tag *)buf);

	return 0;
}

