// this code convert the prefix expression to postfix expression ... Coded by Abhijit Kumar
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

typedef struct n
{
    char info;
    struct n *next;
}block;

block *top=NULL;
block *infix=NULL;
block *stack=NULL;

block* mem_block()
{
    block *nb;
    nb=(block*)malloc(sizeof(block));
    nb->next=NULL;
    return nb;
}

block* push(char info, block *p)
{
    block *nb;
    nb=mem_block();

    if(nb==NULL)
    {
        printf("\nOVERFLOW");
    }

    else
    {
        nb->info=info;
        nb->next=p;
        p=nb;
    }

    return p;
}

block* pop(block *p, char *c)
{
    block *ptr;
    ptr=p;

    if(p==NULL)
    {
        printf("\nUNDERFLOW");
    }

    else
    {
        *c=(ptr->info);
        p=ptr->next;
        free(ptr);
        ptr=NULL;
    }
    return p;
}

void createEq()                     //Input already suitable for PREFIX conversion.
{
    char c;
    top=push('(', top);

    do
    {
        scanf(" %c", &c);
        top=push(c, top);
        
    }while(c!='.');
}

void reverse_equation()             //Input modified that results reverse and makes that suitable for POSTFIX conversion.
{
    block *temp=NULL;
    char ch;

    while(top!=NULL)
    {
        top=pop(top, &ch);
        stack=push(ch, stack);
    }
    
    stack=pop(stack, &ch);

    while(stack!=NULL)
    {
        stack=pop(stack, &ch);
        temp=push(ch, temp);
    }

    top=push(')', top);
    
    while(temp!=NULL)
    {
        temp=pop(temp, &ch);
        top=push(ch, top);
    }

    //display(top);
}

void check_precedence(char c)
{
    char tempc;
    if(stack!=NULL)
    {
        {
            stack=pop(stack, &tempc);

            if(c=='+'||c=='-')
            {
                if(tempc=='+'||tempc=='-'||tempc=='*'||tempc=='/'||tempc=='^')
                {
                    infix=push(tempc, infix);
                }
                else
                {
                    stack=push(tempc, stack);
                }
                stack=push(c, stack);
            }

            else if(c=='*'||c=='/')
            {
                if(tempc=='*'||tempc=='/'||tempc=='^')
                {
                    infix=push(tempc, infix);
                }
                else
                {
                    stack=push(tempc, stack);
                }
                stack=push(c, stack);
            }

            else if(c=='^')
            {
                if(tempc=='^')
                {
                    infix=push(tempc, infix);
                }
                else
                {
                    stack=push(tempc, stack);
                }
                stack=push(c, stack);
            }
        }
    }
    else
    {
        stack=push(c, stack);
    }
}

int char_to_int_postfix_conversion(int check)
{
    char c;
    int i, code, int_value=0;

    infix=pop(infix, &c);
            
    i=0;
    code=0;
    while(c!=',' && c!='#' && infix!=NULL)
    {
        if(c=='-'||c=='&')
        {
            int_value*=-1;
            infix=pop(infix, &c);
        }
        else
        {
            int_value+=((c)*(pow(10, i)));
            code+=((48)*(pow(10, i)));
            infix=pop(infix, &c);
            i++;
        }
    }
    int_value-=code;

    if(check==-1)
    {
        if(c!='#' && infix!=NULL)
        {
            infix=push(',', infix);
        }
        else
        {
            infix=push('#', infix);
        }
    }

    return int_value;
}

int evaluate_calculation(int n1, int n2, char oper)
{
    int result;

    switch(oper)
    {
        case '+':
        {
            result=n1+n2;
            break;
        }

        case '-':
        {
            result=n1-n2;
            break;
        }

        case '*':
        {
            result=n1*n2;
            break;
        }

        case '/':
        {
            result=n1/n2;
            break;
        }

        case '^':
        {
            result=pow(n1,n2);
            break;
        }

        default:
        {
            break;
        }
    }

    return result;
}



void postfix_evaluation()
{
    char c;
    int i, code, k=0;

    while(infix!=NULL)
    {
        infix=pop(infix, &c);
    }

    infix=push('#', infix);

    while(top!=NULL)
    {
        top=pop(top, &c);

        if((c>='a'&&c<='z')||(c>='A'&&c<='Z'))
        {
            printf("Variables present! Hence, evaluation not possible.");
            exit(0);
        }

        else if(c<=57 && c>=48)             //HTML code of char '0' is &#048(or 48 using typecasting). For 9 HTML code is &#057
        {
            while(c!=',' && top->info!='#')
            {
                infix=push(c, infix);
                top=pop(top, &c);
            }
            infix=push(',', infix);
        }

        else if(c=='+'||c=='-'||c=='*'||c=='/'||c=='^')
        {
            char temp=c;
            int n1=0, n2=0, result;

            infix=pop(infix, &c);

            n2=char_to_int_postfix_conversion(0);
            n1=char_to_int_postfix_conversion(-1);
            
            result=evaluate_calculation(n1, n2, temp);

            int power=0;

            if(result<0)
            {
                infix=push('&', infix);
                result*=-1;
            }

            while(pow(10, power)<result)
            {
                power++;
            }

            while(power-1>=0)
            {
                int temp;
                temp=pow(10, power-1);
                if(temp%10!=0)
                {
                    if(temp!=1)
                    {
                        temp++;
                    }
                }
                infix=push((result/temp)+48, infix);
                result=result%temp;
                power--;
            }

            infix=push(',', infix);
            top=pop(top, &c);
        }
    }
    infix=pop(infix, &c);
}

void display(block *p)
{
    
    if(p==NULL)
    {
        printf("EMPTY STACK");
    }
    else
    {
        while(p!=NULL)
        {
            if(p->info=='&')
            {
                printf("-");
            }
            else if(p->info!='#')
            {
                printf("%c", p->info);
            }
            p=p->next;
        }
    }
}

void reverse_display(block *q)
{
    if(q!=NULL)
    {
        reverse_display(q->next);
        if(q->info=='&')
        {
            printf("-");
        }
        else if(q->info!='#')
        {
            printf("%c", q->info);
        }
    }
}

void conversion_evaluation()
{
    // {
    //     int code=0;

        printf("\n\t3. Postfix Equation Evaluation.");
        // printf("\n\t0. EXIT.");

        // printf("\n\tEnter Code: ");
        // scanf("%d", &code); 

        // while(code!=0||code>3) 
        // {
        //     switch(code)
        //     {
        //         case 1:
        //         {
        //             infix_to_prefix_conversion();
        //             printf("\n\nEquivalent Prefix Expression: \n");
        //             display(infix);
        //             break;
        //         }
                
        //         case 2:
        //         {
        //             reverse_equation();
        //             infix_to_postfix_conversion();
        //             printf("\n\nEquivalent Postfix Expression: \n");
        //             reverse_display(infix);
        //             break;
        //         }

        //         case 3:
        //         {
                    reverse_equation();
                    postfix_evaluation();
                    printf("\n\nSimplified Result for Entered Postfix Expression: \t");
                    reverse_display(infix);
                //     break;
                // }

            //     case 0:
            //     {
            //         char c;
            //         while(top!=NULL)
            //         {
            //             top=pop(top, &c);
            //         }
            //         exit(0);
            //         break;
            //     }
            
            //     default:
            //     {
            //         printf("\nInvalid Code!");
            //         break;
            //     }
            // }

            // if(code<=3)
            // {
            //     break;
            // }

            // printf("\nEnter Code: ");
            // scanf("%d", &code); 
    //     } 
    // } 
}



int main()
{
    char ch;
    int code=3;

    // do
    // {
        // if(code==3)
        // {
            printf("\n\n\nPrefix/Postfix Calculator/Converter\n");
            printf("\n1. Insert Equation.\n2. Display\n3. Conversion/Evaluation\n0. Exit\n");
        // }

        // printf("\nEnter Code: ");
        // scanf("%d", &code);

        // switch(code)
        // {
        //     case 1:
        //     {
                printf("\nNote: \n1. Use a fullstop(.) followed by an 'Enter' to insert an expression.\n2. For Prefix/Postfix Evaluation, ");
                printf("Please enter a comma(,) after every value/symbol.\n\nEnter the Expression: \n");
                createEq();
                top=pop(top, &ch);
        //         break;
        //     }
            
        //     case 2:
        //     {
        //         printf("\n");
        //         reverse_display(top);
        //         printf(")\n");
        //         break;
        //     }

            // case 3:
            // {
                conversion_evaluation();
                // break;
            // }

    //         case 0:
    //         {
    //             exit(0);
    //             break;
    //         }
        
    //         default:
    //         {
    //             printf("\nInvalid Code!");
    //             break;
    //         }   
    //     }
    // }while(code!=0);

    return 0;
}
