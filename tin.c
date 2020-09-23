#include <stdio.h>
#include <string.h>
#include <curses.h>
#include <mysql.h>
#include <stdlib.h>

MYSQL *conn;
MYSQL_RES *res;
MYSQL_ROW row;
int lid = 0;
int pr1 = 0;
int pr2 = 0;
int pr8 = 0;
int tp = 0;

void avail();
void adminAvail();
void book();
void test();
void cancel();
void bookedByUser();
void cancelAll();
void price();
void changePrice();

void signup();
int login();
void forgetPass();
void forgetId();

int menu1();
int menu();
int adminmenu();

int main()
{
	int i = 1, p = 150, ch, ch2, ls;
	char q[100];
	char h1, h2;
	char *st = "avail";

	char *server = "localhost";
	char *user = "root";
	char *pass = "Panesar@79904";
	char *db = "theater";

	conn = mysql_init(NULL);

	if (!mysql_real_connect(conn, server, user, pass, db, 0, NULL, 0))
	{
		fprintf(stderr, "here %s\n", mysql_error(conn));
		exit(-1);
	}

	/*
	for(i=1;i<=100;i++)
	{

		sprintf(q,"INSERT INTO ticket(seatNo,price,status)VALUES(%d,%d,\"%s\")",i,p,st);


		if(mysql_query(conn,q))
		{
			fprintf(stderr,"%s\n",mysql_error(conn));
			exit(-1);
		}
	}


	avail();

	
	book();

 
 	signup();
    login();
	price();	
	avail();
	book();
	adminAvail();
	 */
	price();
	while (1)
	{
		system("clear");

		ch = menu1();
		switch (ch)
		{
		case 1:
			ls = login();
			if (ls == 1)
			{
				do
				{
					system("clear");
					ch2 = menu();
					switch (ch2)
					{
					case 1:
						tp = 0;
						book();
						break;
					case 2:
						cancel();
						break;
					case 3:
						bookedByUser();
						break;
					case 4:
						avail();
						break;
					case 5:
						break;
					case 6:
						exit(-1);
					case 7:
						tp = 0;
						test();
					}
					fflush(stdin);
					getchar();
					getchar();
				} while (ch2 != 5);
			}
			if (ls == 2)
			{
				do
				{
					ch2 = adminmenu();
					switch (ch2)
					{
					case 1:
						cancelAll();
						break;
					case 2:
						changePrice();
						break;
					case 3:
						adminAvail();
						break;
					case 4:
						break;
					case 5:
						exit(-1);
					}
				} while (ch2 != 4);
			}
			break;
		case 2:
			signup();
			break;
		case 3:
			forgetPass();
			break;
		case 4:
			forgetId();
			break;
		case 5:
			exit(-1);
			break;
		default:
			printf("Enter correct choice (1 to 5) \n");
		}
		fflush(stdin);
		getchar();
		getchar();
	}
	//signup();
	//login();
	/**/
	mysql_free_result(res);
	mysql_close(conn);
	return 0;
}

void book()
{
	avail();
	int sn[30], n, flag = 0, i;
	char q[100];
	char *st = "Booked";
	char getst[30][10];
	int uid = lid;
	printf("How many seat you do want to book : ");
	scanf("%d", &n);
	printf("Enter seat number : ");
	for (i = 0; i < n; i++)
	{
		scanf("%d", &sn[i]);
	}
	for (i = 0; i < n; i++)
	{
		sprintf(q, "Select status from ticket where seatNo=%d", sn[i]);
		if (mysql_query(conn, q))
		{
			fprintf(stderr, "%s\n", mysql_error(conn));
			exit(-1);
		}
		res = mysql_use_result(conn);
		while ((row = mysql_fetch_row(res)) != NULL)
		{

			strcpy(getst[i], row[0]);
		}
		if (sn[i] < 5)
		{
			tp += pr1;
		}
		else if (sn[i] < 69)
		{
			tp += pr2;
		}
		else if (sn[i] < 5)
		{
			tp += pr1;
		}
	}
	for (i = 0; i < n; i++)
	{
		if (strcmp(getst[i], "avail") != 0)
		{
			flag = 1;
			printf("\nseat no %d is aleardy booked ", sn[i]);
		}
	}
	if (flag == 0)
	{
		for (i = 0; i < n; i++)
		{
			sprintf(q, "UPDATE ticket SET status=\"%s\",bookedBY=%d where seatNo=%d", st, uid, sn[i]);
			if (mysql_query(conn, q))
			{
				fprintf(stderr, "%s\n", mysql_error(conn));
				exit(-1);
			}
		}
		printf("Booked Successfully");
		printf("\nTotal Price of your booking is : %d ", tp);
	}
	else
	{
		printf("please choose again");
	}
}

void avail()
{
	char q[100];
	int i = 1;
	sprintf(q, "SELECT * FROM ticket ");

	if (mysql_query(conn, q))
	{
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(-1);
	}

	res = mysql_use_result(conn);

	printf("\n\n aviable seats are \n\n\n");
	while ((row = mysql_fetch_row(res)) != NULL)
	{
		if (i == 1)
		{
			printf("\t\t\t\t----------------------------RS.%d----------------------------\n", pr1);
			printf("\n\t\t\t\t");
		}
		if (i == 5)
		{
			printf("\n\n\t\t\t\t----------------------------RS.%d----------------------------\n\n  ", pr2);
		}

		if (i == 69)
		{
			printf("\n\n\t\t\t\t----------------------------RS.%d----------------------------\n\n  ", pr8);
		}
		printf("%3s . %6s\t", row[0], row[2]);
		if ((i - 4) % 8 == 0)
		{
			printf("\n  ");
		}
		i++;
	}
	printf("\n\t\t\t\t-----------------------Secreen this side------------------------\n\n");
}

void adminAvail()
{
	char q[100];
	int i = 1;
	sprintf(q, "SELECT * FROM ticket ");

	if (mysql_query(conn, q))
	{
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(-1);
	}

	res = mysql_use_result(conn);

	printf("\n\n\t\t aviable seats are \n");
	printf("Seats with (Null) are available and Booked seat are showed seat num and userID booked seat \n\n");
	while ((row = mysql_fetch_row(res)) != NULL)
	{
		if (i == 1)
		{
			printf("\t\t\t\t----------------------------RS.%d----------------------------\n", pr1);
			printf("\n\t\t\t\t");
		}
		if (i == 5)
		{
			printf("\n\n\t\t\t\t----------------------------RS.%d----------------------------\n\n  ", pr2);
		}

		if (i == 69)
		{
			printf("\n\n\t\t\t\t----------------------------RS.%d----------------------------\n\n  ", pr8);
		}
		printf("%3s . %6s\t", row[0], row[3]);
		if ((i - 4) % 8 == 0)
		{
			printf("\n  ");
		}
		i++;
	}
	printf("\n\t\t\t\t-----------------------Secreen this side------------------------\n\n");
}

void test()
{
	avail();
	int sn;
	int tp = 0;
	char q[100];
	char *st = "Booked";
	char getst[10];
	int uid = lid;
	printf("Enter seat number : ");
	scanf("%d", &sn);
	sprintf(q, "Select status from ticket where seatNo=%d", sn);
	if (mysql_query(conn, q))
	{
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(-1);
	}
	res = mysql_use_result(conn);

	while ((row = mysql_fetch_row(res)) != NULL)
	{
		strcpy(getst, row[0]);
	}
	if (strcmp(getst, "avail") == 0)
	{
		sprintf(q, "UPDATE ticket SET status=\"%s\",bookedBY=%d where seatNo=%d", st, uid, sn);
		if (mysql_query(conn, q))
		{
			fprintf(stderr, "%s\n", mysql_error(conn));
			exit(-1);
		}
		printf("Booked Successfully");
	}
	else
	{
		printf("This seat is already booked choose other seat");
	}
}

void cancelAll()
{
	char *st = "avail";
	char q[100];
	sprintf(q, "UPDATE ticket SET status=\"%s\",bookedBY=NULL ", st);
	if (mysql_query(conn, q))
	{
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(-1);
	}
}

void price()
{
	char q[100];
	int i = 1;
	sprintf(q, "Select price from price");
	if (mysql_query(conn, q))
	{
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(-1);
	}
	res = mysql_use_result(conn);

	while ((row = mysql_fetch_row(res)) != NULL)
	{
		if (i == 1)
		{
			pr1 = atoi(row[0]);
		}
		if (i == 2)
		{
			pr2 = atoi(row[0]);
		}
		if (i == 3)
		{
			pr8 = atoi(row[0]);
		}
		i++;
	}
}

void changePrice()
{
	char q[100];
	printf("Old price of row1 is : %d\nEnter new price if you want to change(else enter old) : ", pr1);
	scanf("%d", &pr1);
	sprintf(q, "UPDATE price SET price=%d where id=1", pr1);
	if (mysql_query(conn, q))
	{
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(-1);
	}
	printf("Old price of row2 to row6 is : %d\nEnter new price if you want to change(else enter old) : ", pr2);
	scanf("%d", &pr2);
	sprintf(q, "UPDATE price SET price=%d where id=2", pr2);
	if (mysql_query(conn, q))
	{
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(-1);
	}
	printf("Old price of row1 is : %d\nEnter new price if you want to change(else enter old) : ", pr8);
	scanf("%d", &pr8);
	sprintf(q, "UPDATE price SET price=%d where id=8", pr8);
	if (mysql_query(conn, q))
	{
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(-1);
	}
}

void cancel()
{
	char q[100];
	char st;
	int count;
	int csn;
	int csnr;
	char *sta = "avail";
	sprintf(q, "SELECT COUNT(*) from ticket where bookedBy=%d", lid);
	if (mysql_query(conn, q))
	{
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(-1);
	}
	res = mysql_use_result(conn);
	while ((row = mysql_fetch_row(res)) != NULL)
	{
		count = atoi(row[0]);
	}
	if (count == 0)
	{
		printf("you have not booked any seat yet");
		return;
	}
	sprintf(q, "SELECT * from ticket where bookedBy=%d", lid);
	if (mysql_query(conn, q))
	{
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(-1);
	}
	res = mysql_use_result(conn);
	printf("\nSeat booked by you\n");
	while ((row = mysql_fetch_row(res)) != NULL)
	{
		printf("%s\t", row[0]);
	}
	printf("\nenter seatno you want to cancle : ");
	scanf("%d", &csn);
	sprintf(q, "SELECT * from ticket where seatNo=%d", csn);
	if (mysql_query(conn, q))
	{
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(-1);
	}
	res = mysql_use_result(conn);
	while ((row = mysql_fetch_row(res)) != NULL)
	{
		if (row[3] != NULL)
		{
			csnr = atoi(row[3]);
		}
		else
		{
			printf("this seat is not booked .Retry");
			return;
		}
	}
	if (csnr == lid)
	{
		sprintf(q, "UPDATE ticket SET status=\"%s\",bookedBY=NULL where seatNo=%d", sta, csn);
		if (mysql_query(conn, q))
		{
			fprintf(stderr, "%s\n", mysql_error(conn));
			exit(-1);
		}
		printf("your ticket is cancle successfully");
	}
	else
	{
		printf("this is not booked by you");
	}
}

void bookedByUser()
{
	char q[100];
	char st;
	sprintf(q, "SELECT * from ticket where bookedBy=%d", lid);
	if (mysql_query(conn, q))
	{
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(-1);
	}
	res = mysql_use_result(conn);
	printf("\nSeat booked by you\n");
	while ((row = mysql_fetch_row(res)) != NULL)
	{
		printf("%s\t", row[0]);
	}
}

void signup()
{
	char un[20];
	char pass[20];
	char q[100];
	char mobn[11];
	char st;

	printf("Enter username : ");
	scanf("%s", un);
	printf("Enter password : ");
	scanf("%s", pass);
	printf("Enter mobile number : ");
	scanf("%s", mobn);

	sprintf(q, "SELECT COUNT(*) FROM user where uName=\"%s\" AND uPass=\"%s\" AND mobnum=\"%s\"", un, pass, mobn);

	if (mysql_query(conn, q))
	{
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(-1);
	}

	res = mysql_use_result(conn);

	while ((row = mysql_fetch_row(res)) != NULL)
	{
		st = row[0][0];
	}
	if (st != '0')
	{
		printf("\n\nUser alerdy exist \n\n");
		return;
	}

	sprintf(q, "INSERT INTO user(uName,uPass,mobNum) VALUES (\"%s\",\"%s\",\"%s\")", un, pass, mobn);

	if (mysql_query(conn, q))
	{
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(-1);
	}

	sprintf(q, "SELECT * FROM user where uName=\"%s\" AND uPass=\"%s\" AND mobnum=\"%s\"", un, pass, mobn);

	if (mysql_query(conn, q))
	{
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(-1);
	}

	res = mysql_use_result(conn);

	printf("\nUser created succesfully \nYour user id is : ");
	while ((row = mysql_fetch_row(res)) != NULL)
	{
		printf("%s\n", row[0]);
	}
}

int login()
{
	char q[100], st;
	int uid;
	char pass[20];
	printf("\nEnter User_ID and Password : ");
	scanf("%d%s", &uid, pass);

	sprintf(q, "SELECT COUNT(*) from user where id=%d AND uPass=\"%s\"", uid, pass);

	if (mysql_query(conn, q))
	{

		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(-1);
	}

	res = mysql_use_result(conn);

	while ((row = mysql_fetch_row(res)) != NULL)
	{
		st = row[0][0];
	}

	if (st == '1')
	{
		lid = uid;
		printf("Login succesfully .");
		if (uid == 1)
		{
			return 2;
		}
		return 1;
	}
	else
	{
		printf("Enter correct user name and password ");
	}
}

void forgetPass()
{
	int ud;
	char mobn[11];
	char q[100];
	char st;

	printf("Enter user_ID : ");
	scanf("%d", &ud);
	printf("Enter mobile number : ");
	scanf("%s", mobn);

	sprintf(q, "SELECT COUNT(*) FROM user where id=%d AND mobnum=\"%s\"", ud, mobn);

	if (mysql_query(conn, q))
	{
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(-1);
	}

	res = mysql_use_result(conn);

	while ((row = mysql_fetch_row(res)) != NULL)
	{
		st = row[0][0];
	}
	if (st == '0')
	{
		printf("\n\nNo Record found by this user_Id and mobile number \n\n");
		return;
	}

	sprintf(q, "SELECT uPass FROM user where id=%d AND mobnum=\"%s\"", ud, mobn);

	if (mysql_query(conn, q))
	{
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(-1);
	}
	res = mysql_use_result(conn);
	printf("\nyour Record Found \nYour password  is : ");
	while ((row = mysql_fetch_row(res)) != NULL)
	{
		printf("%s\n", row[0]);
	}
}

void forgetId()
{
	char nm[20];
	char mobn[11];
	char q[100];
	char st;

	printf("Enter Name : ");
	scanf("%s", nm);
	printf("Enter mobile number : ");
	scanf("%s", mobn);

	sprintf(q, "SELECT COUNT(*) FROM user where uName=\"%s\" AND mobnum=\"%s\"", nm, mobn);

	if (mysql_query(conn, q))
	{
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(-1);
	}

	res = mysql_use_result(conn);

	while ((row = mysql_fetch_row(res)) != NULL)
	{
		st = row[0][0];
	}
	if (st == '0')
	{
		printf("\n\nNo Record found by this user_Id and mobile number \n\n");
		return;
	}

	sprintf(q, "SELECT id FROM user where uName=\"%s\" AND mobnum=\"%s\"", nm, mobn);

	if (mysql_query(conn, q))
	{
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(-1);
	}
	res = mysql_use_result(conn);
	printf("\nyour Record Found \nYour user_id is : ");
	while ((row = mysql_fetch_row(res)) != NULL)
	{
		printf("%s\n", row[0]);
	}
}

int menu1()
{
	int ch;
	printf("\n1)FOR LOGIN");
	printf("\n2)FOR SIGNUP");
	printf("\n3)FOR Forget Password");
	printf("\n4)FOR Forget userId");
	printf("\n5)FOR EXIT : ");
	scanf("%d", &ch);
	return ch;
}

int menu()
{
	int ch;
	printf("\n\n\n1)FOR Book Ticket");
	printf("\n2)FOR Cancel seat");
	printf("\n3)FOR See Booked by you");
	printf("\n4)FOR See available");
	printf("\n5)FOR logout ");
	printf("\n6)FOR EXIT : ");
	scanf("%d", &ch);
	return ch;
}

int adminmenu()
{
	int ch;
	printf("\n\n\n1)FOR clear All booked Ticket");
	printf("\n2)for changing price of seats.");
	printf("\n3)FOR See all booked and available");
	printf("\n4)FOR logout ");
	printf("\n5)FOR EXIT : ");
	scanf("%d", &ch);
	return ch;
}
