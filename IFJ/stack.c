
#include "stack.h"

void SInit (tStack *s)
{
	if (s == NULL)
	{
		 exit(99);
	}
	s->top = -1;
	s->data = NULL;

}

int SEmpty (tStack *s)
{
	return (s->top == -1);
}

int STop (tStack *s)
{
	if (SEmpty(s) == 1)
	{
		 exit(99);
	}
	return (s->data[s->top]);
}

void SPush (tStack *s, int data, int data_type)
{
	fprintf(stderr,"tu\n");
	s->top = s->top + 1;
	if (s->top == 0)
	{	
        s->data = (int *) malloc (sizeof(int));
        s->data_type = (int *) malloc (sizeof(int));
        s->is_variable = (int *) malloc (sizeof(int));
    }    
    else
    {	
    	s->data = (int *) realloc (s->data, sizeof(int) * (s->top + 1));
    	s->data_type = (int *) realloc (s->data_type, sizeof(int) * (s->top + 1));
    	s->is_variable = (int *) realloc (s->is_variable, sizeof(int) * (s->top + 1));
    }	
    if ((s->data == NULL) || (s->data_type == NULL))
    {
    	 exit(99);
    }
    else
    {	
    	s->data[s->top] = data;
    	s->data_type[s->top] = data_type;
    	s->value[s->top] = NULL;
    	s->is_variable[s->top] = 0;
    }

}
int STopPop (tStack *s)
{
 	if ((s->data == NULL) || (s->top == -1) || (s->data_type == NULL) || (s->is_variable == NULL))
 	{
 		 exit(99);
 	}
    else
    {
		int c = s->data[s->top];
		s->value[s->top] = NULL;
		s->top = s->top - 1;
		s->data = (int *) realloc (s->data, sizeof(int) * (s->top +1));
		s->data_type = (int *) realloc (s->data_type, sizeof(int) * (s->top + 1));
		s->is_variable = (int *) realloc (s->is_variable, sizeof(int) * (s->top + 1));

		if ((s->data == NULL) || (s->data_type == NULL) || (s->is_variable == NULL)) 
	{
		 exit(99);
	}
	else
		return (c);
}
}

int STopSec (tStack *s)
{
	if (((s->top == -1) || (s->data == NULL)) || (s->data_type == NULL) || (s->is_variable == NULL))
	{
	 	 exit(99);
	}
	else
	 	return (s->data[s->top - 1]);
}

void SFree(tStack *s)
{
	if ((s->data == NULL) || (s->data_type == NULL) || (s->is_variable == NULL))
	{
		 exit(99);
	}
	else
	{
		free(s->data);
		s->data = NULL;
	}
}

void SPrint(tStack *s)
{
	if (((s->data == NULL) || (s->top == -1)) || (s->data_type == NULL) || (s->is_variable == NULL))
	{
		 exit(99);
	}
	else
	{
      int i;
      for (i = s->top; i > -1; i--)
      	 fprintf(stderr,"%d\n",s->data[i]);
}
}