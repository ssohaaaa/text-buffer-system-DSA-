#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#define max 100

struct node
{
    char data;
    struct node *prev;
    struct node *next;
};

struct node *head=NULL;
struct node *tail=NULL;

char savedmorse[500]="";
int morseexists=0;

char oldtext[100];
int replaced = 0;

char newtext[100];
int redoavailable = 0;

void insert()
{
    char text[100];
    int i;

    printf("\nenter text: ");
    scanf(" %99[^\n]", text);
    for(i=0; text[i]!='\0';i++)
    {
        struct node *newnode=malloc(sizeof(struct node));

        newnode->data=text[i];
        newnode->next=NULL;
        newnode->prev=tail;

        if(head==NULL)
        {
            head=newnode;
            tail=newnode;
        }
        else{
            tail->next=newnode;
            tail=newnode;
        }
    }
    printf("\ntext inserted");
}

void display()
{
    struct node *temp=head;
    if(head==NULL)
    {
        printf("\nempty");
        return;
    }
    printf("\ntext: ");
    while(temp!=NULL)
    {
        printf("%c",temp->data);
        temp=temp->next;
    }
}

void deletetext()
{
    struct node *temp;
    struct node *start;
    struct node *del;
    char word[100];
    int i;

    if(head==NULL)
    {
        printf("\nempty text");
        return;
    }

    printf("\nenter word to delete: ");
    scanf("%99s",word);

    temp=head;

    while(temp!=NULL)
    {
        if(temp->data==word[0]&&(temp->prev==NULL || temp->prev->data==' '))
        {
            start=temp;
            i=0;
            while(temp!=NULL && word[i]!='\0' && temp->data==word[i])
            {
                temp=temp->next;
                i++;
            }
            if(word[i]=='\0'&&(temp==NULL || temp->data==' '))
            {
                while(start!=temp)
                {
                    del=start;
                    start=start->next;
                    if(del->prev!=NULL)
                    {
                        del->prev->next=del->next;
                    }
                    else
                    {
                        head=del->next;
                    }
                    if(del->next!=NULL)
                    {
                        del->next->prev=del->prev;
                    }
                    else
                    {
                        tail=del->prev;
                    }
                    free(del);

                }
                printf("\nword deleted");
                return;

            }

        }
        temp=temp->next;
    }
    printf("\nword not found");
}

void savetext(char text[])
{
    struct node *temp = head;
    int i = 0;

    while(temp != NULL && i < 99)
    {
        text[i++] = temp->data;
        temp = temp->next;
    }

    text[i] = '\0';
}

void replace()
{
    struct node *temp;
    char text[100], result[100], old[50], new[50];
    int i = 0, j = 0, k, len, found = 0;

    if(head == NULL)
    {
        printf("\nno text");
        return;
    }

    savetext(text);

    printf("\nenter word to replace: ");
    scanf("%49s", old);

    printf("enter new word: ");
    scanf("%49s", new);

    len = strlen(old);

    while(text[i] != '\0' && j < 99)
    {
        int match = 1;

        for(k = 0; k < len; k++)
        {
            if(text[i + k] == '\0' ||
               tolower((unsigned char)text[i + k]) !=
               tolower((unsigned char)old[k]))
            {
                match = 0;
                break;
            }
        }

        if(match &&
           (i == 0 || !isalnum((unsigned char)text[i - 1])) &&
           !isalnum((unsigned char)text[i + len]))
        {
            if(found == 0)
            {
                strcpy(oldtext, text);
                replaced = 1;
            }

            found = 1;

            for(k = 0; new[k] != '\0' && j < 99; k++)
                result[j++] = new[k];

            i += len;
        }
        else
        {
            result[j++] = text[i++];
        }
    }

    result[j] = '\0';
    strcpy(newtext, result);
    redoavailable = 1;

    if(found == 0)
    {
        printf("\nword not found");
        return;
    }

    temp = head;

    while(temp != NULL)
    {
        struct node *nextnode = temp->next;
        free(temp);
        temp = nextnode;
    }

    head = NULL;
    tail = NULL;

    for(i = 0; result[i] != '\0'; i++)
    {
        struct node *newnode = malloc(sizeof(struct node));

        if(newnode == NULL)
            return;

        newnode->data = result[i];
        newnode->next = NULL;
        newnode->prev = tail;

        if(tail != NULL)
            tail->next = newnode;
        else
            head = newnode;

        tail = newnode;
    }

    printf("\nword replaced");
}

void undo()
{
    struct node *temp=head;
    int i;

    if(replaced==0)
    {
        printf("\nnothing to undo");
        return;
    }
    temp=head;
    while(temp!=NULL)
    {
        struct node *nextnode=temp->next;
        free(temp);
        temp=nextnode;
    }
    head=NULL;
    tail=NULL;

    for(i=0;oldtext[i]!='\0';i++)
    {
        struct node *newnode=malloc(sizeof(struct node));

        if(newnode==NULL)
        {
            printf("\nnot allocated");
            return;
        }

        newnode->data=oldtext[i];
        newnode->next=NULL;
        newnode->prev=tail;

        if(head==NULL)
        {
            head=newnode;
        }
        else{
            tail->next=newnode;
        }
        tail=newnode;
    }
    replaced=0;
    printf("\nreplace undone");
}

void redo()
{
    struct node *temp=head;
    int i;
    if(redoavailable==0)
    {
        printf("\nnothing to redo");
        return;
    }

    temp=head;

    while(temp!=NULL)
    {
        struct node *nextnode=temp->next;
        free(temp);
        temp=nextnode;
    }

    head=NULL;
    tail=NULL;

    for(i=0; newtext[i]!='\0';i++)
    {
        struct node *newnode=malloc(sizeof(struct node));

        if(newnode==NULL)
        {
            printf("\nallocation failed");
            return;
        }

        newnode->data=newtext[i];
        newnode->next=NULL;
        newnode->prev=tail;

        if(head==NULL)
            head=newnode;
        else
            tail->next=newnode;

        tail=newnode;
    }
    replaced=1;
    redoavailable=0;
    printf("\nundo redone");
}

void find()
{
    char buffer[100], word[100];
    int i=0, j, pos=1, found=0;
    char temp[100];

    savetext(buffer);

    printf("\nenter word to find: ");
    scanf("%99s",word);

    while(buffer[i]!='\0')
    {
        j=0;

        while(buffer[i]==' ')
            i++;
        if(buffer[i]=='\0')
            break;
        while(buffer[i]!=' ' && buffer[i]!='\0')
            temp[j++]=buffer[i++];

        temp[j]='\0';

        if(strcmp(temp,word)==0)
        {
            printf("\nword found at position %d",pos);
            found=1;
            break;
        }
        pos++;
    }
    if(found==0)
    {
        printf("\nword not foudn");
    }

}

void texttomorse()
{
    char text[100];
    char *morse[]= {
        ".-", "-...", "-.-.", "-..", ".", "..-.", "--.",
        "....", "..", ".---", "-.-", ".-..", "--", "-.",
        "---", ".--.", "--.-", ".-.", "...", "-", "..-",
        "...-", ".--", "-..-", "-.--", "--.."
    };

    struct node *temp=head;

    int i;

    if(head==NULL)
    {
        printf("\nempty list");
        return;
    }
    savetext(text);
    savedmorse[0]='\0';
    for(i=0;text[i]!='\0';i++)
    {
        char code[10];

        if(isalpha((unsigned char)text[i]))
        {
            strcpy(code, morse[toupper((unsigned char)text[i])-'A']);
            strcat(savedmorse,code);
            strcat(savedmorse," ");
        }
        else if(text[i]==' ')
        {
            strcat(savedmorse,"/");
        }
    }
        while(temp!=NULL)
        {
            struct node *nextnode=temp->next;
            free(temp);
            temp=nextnode;
        }
        head=NULL;
        tail=NULL;

        for(i=0;savedmorse[i]!='\0';i++)
        {
            struct node *newnode=malloc(sizeof(struct node));
            if(newnode==NULL)
            {
                printf("\nallocation failed");
                return;
            }

            newnode->data=savedmorse[i];
            newnode->next=NULL;
            newnode->prev=tail;

            if(head==NULL)
                head=newnode;
            else
                tail->next=newnode;

            tail=newnode;

        }



    morseexists=1;
    printf("\nconverted to morse");
}

void morsetotext()
{
    char text[500];
    char word[10];
    char *morse[] = {
        ".-", "-...", "-.-.", "-..", ".", "..-.", "--.",
        "....", "..", ".---", "-.-", ".-..", "--", "-.",
        "---", ".--.", "--.-", ".-.", "...", "-", "..-",
        "...-", ".--", "-..-", "-.--", "--.."
    };

    char letters[] = "abcdefghijklmnopqrstuvwxyz";
    int i = 0, j = 0, k, found;
    struct node *temp = head;

    if(head == NULL)
    {
        printf("\nempty text");
        return;
    }

    savetext(text);

    while(temp != NULL)
    {
        struct node *nextnode = temp->next;
        free(temp);
        temp = nextnode;
    }

    head = NULL;
    tail = NULL;

    while(text[i] != '\0')
    {
        if(text[i] == '/')
        {
            i++;
            word[j] = '\0';

            if(j > 0)
            {
                found = 0;

                for(k = 0; k < 26; k++)
                {
                    if(strcmp(word, morse[k]) == 0)
                    {
                        struct node *newnode = malloc(sizeof(struct node));

                        if(newnode == NULL)
                            return;

                        newnode->data = letters[k];
                        newnode->next = NULL;
                        newnode->prev = tail;

                        if(head == NULL)
                            head = newnode;
                        else
                            tail->next = newnode;

                        tail = newnode;
                        found = 1;
                        break;
                    }
                }

                j = 0;
            }

            struct node *newnode = malloc(sizeof(struct node));

            if(newnode == NULL)
                return;

            newnode->data = ' ';
            newnode->next = NULL;
            newnode->prev = tail;

            if(head == NULL)
                head = newnode;
            else
                tail->next = newnode;

            tail = newnode;
        }
        else if(text[i] == ' ')
        {
            if(j > 0)
            {
                word[j] = '\0';
                found = 0;

                for(k = 0; k < 26; k++)
                {
                    if(strcmp(word, morse[k]) == 0)
                    {
                        struct node *newnode = malloc(sizeof(struct node));

                        if(newnode == NULL)
                            return;

                        newnode->data = letters[k];
                        newnode->next = NULL;
                        newnode->prev = tail;

                        if(head == NULL)
                            head = newnode;
                        else
                            tail->next = newnode;

                        tail = newnode;
                        found = 1;
                        break;
                    }
                }

                j = 0;
            }

            i++;
        }
        else
        {
            if(j < 9)
                word[j++] = text[i];

            i++;
        }
    }

    if(j > 0)
    {
        word[j] = '\0';

        for(k = 0; k < 26; k++)
        {
            if(strcmp(word, morse[k]) == 0)
            {
                struct node *newnode = malloc(sizeof(struct node));

                if(newnode == NULL)
                    return;

                newnode->data = letters[k];
                newnode->next = NULL;
                newnode->prev = tail;

                if(head == NULL)
                    head = newnode;
                else
                    tail->next = newnode;

                tail = newnode;
                break;
            }
        }
    }

    morseexists = 0;
    printf("\nmorse converted to text");
}

int hcount=0;
char history[100][50];
void showhistory()
{
    int i;

    if(hcount==0)
    {
        printf("\nno command history ");
        return;
    }

    printf("\ncommand history: ");
    for(i=0;i<hcount;i++)
    {
        printf("\n%d: %s",i+1,history[i]);
    }
}

void wordfrequency()
{
    char text[100], word[100][100];
    int i=0, j, count, total=0, k;

    savetext(text);

    if(head==NULL)
    {
        printf("\nempty list");
        return;
    }

    while(text[i]!='\0')
    {
        j=0;
        while(text[i]==' ')
            i++;
        if(text[i]=='\0')
            break;
        while(text[i]!=' ' && text[i]!='\0')
            word[total][j++]=text[i++];

        word[total][j]='\0';
        total++;

    }

    for(i=0;i<total;i++)
    {
        count=1;
        if(word[i][0]=='\0')
            continue;

        for(j=i+1;j<total;j++)
        {
            if(strcmp(word[i],word[j])==0)
            {
                count++;
                word[j][0]='\0';
            }
        }
        printf("\n%s : %d ",word[i],count);
    }
}



int main()
{
    int choice;
    do
    {
       printf("\n\n ~ text buffer system ~");
        printf("\n 1. insert text ");
        printf("\n 2. delete text ");
        printf("\n 3. replace word ");
        printf("\n 4. undo replace ");
        printf("\n 5. redo ");
        printf("\n 6. find word ");
        printf("\n 7. show command history ");
        printf("\n 8. show word frequency ");
        printf("\n 9. convert text to morse code ");
        printf("\n 10. convert morse code to text ");
        printf("\n 11. display ");
        printf("\n 12. exit ");

        printf("\n\nenter choice: ");
        scanf("%d",&choice);

         if(choice>=1 && choice<=11 && hcount<100)
        {
            switch(choice)
            {
                case 1: strcpy(history[hcount++],"insert text");break;
                case 2: strcpy(history[hcount++],"delete text");break;
                case 3: strcpy(history[hcount++],"replace word");break;
                case 4: strcpy(history[hcount++],"undo");break;
                case 5: strcpy(history[hcount++],"redo");break;
                case 6: strcpy(history[hcount++],"find word");break;
                case 7: strcpy(history[hcount++],"show command history");break;
                case 8: strcpy(history[hcount++],"show word frequency");break;
                case 9: strcpy(history[hcount++],"convert text to morse code");break;
                case 10: strcpy(history[hcount++],"convert morse code to text");break;
                case 11: strcpy(history[hcount++],"display");break;
            }
        }


        switch (choice)
        {
        case 1:
            insert();
            break;

        case 2:
            deletetext();
            break;

        case 3:
            replace();
            break;

        case 4:
            undo();
            break;

        case 5:
            redo();
            break;

        case 6:
            find();
                break;

        case 7:
            showhistory();
            break;

        case 8:
            wordfrequency();
            break;

        case 9:
            texttomorse();
            break;

        case 10:
            morsetotext();
            break;

        case 11:
            display();
            break;

        case 12:
            printf("exiting");
            break;

        }
    }while(choice!=12);

    return 0;
}
