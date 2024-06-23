#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<windows.h>
#include <time.h>

int PHealthTime = 0;
int THealthTime = 0;
int Pweight = 0;
int Tweight = 0;
int Initweight = 0;

typedef struct Exercise{
    char ExerciseName[31];
    int SetCount;
    int Count;
    int Weight;
} Exercise;

typedef struct Node {
    char Date[7];
    Exercise exercise[10];
    int Time;
    struct Node* next;
} Node;

typedef struct Stats{
    int totalSets;
    int totalCount;
    int totalWeight;
    int totalTime;
    int totalVolume;
} Stats;

Node* createNode(char* date, Exercise exercises[10], int Time) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("메모리 할당 실패\n");
        return NULL;
    }
    strcpy(newNode->Date, date);
    for (int i = 0; i < 10; i++) {
        newNode->exercise[i] = exercises[i];
    }
    newNode->Time = Time;
    newNode->next = NULL;
    return newNode;
}
void AddExRecord(Node** head, char* date, Exercise exercises[10], int Time) {
    Node* newNode = createNode(date, exercises, Time);
    if (newNode == NULL) {
        return;
    }

    if (*head == NULL) {
        *head = newNode;
        return;
    }

    Node* temp = *head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = newNode;
}
void InitExercises(Exercise exercises[10]) {
    for (int i = 0; i < 10; i++) {
        strcpy(exercises[i].ExerciseName, "");
        exercises[i].SetCount = 0;
        exercises[i].Count = 0;
        exercises[i].Weight = 0;
    }
}
void GetExercises(Exercise exercises[10]) {
    for (int i = 0; i < 10; i++) {
        printf("운동 이름을 입력하세요 (최대 30자(한글은 15자), 종료하려면 'end' 입력): ");
        scanf("%s", exercises[i].ExerciseName);
        if (strcmp(exercises[i].ExerciseName, "end") == 0) {
            strcpy(exercises[i].ExerciseName, "");
            break;
        }
        printf("세트 수를 입력하세요: ");
        scanf("%d", &exercises[i].SetCount);
        printf("횟수를 입력하세요: ");
        scanf("%d", &exercises[i].Count);
        printf("무게를 입력하세요(없으면 0 입력): ");
        scanf("%d", &exercises[i].Weight);
    }
}
void printRecords(Node* head) {
    Node* temp = head;
    printf("=========================================================================================================\n");
    while (temp != NULL) {
        printf("날짜: %s\n", temp->Date);
        for (int i = 0; i < 10 && temp->exercise[i].ExerciseName[0] != '\0'; i++) {
            printf("[%d]운동: %s, 세트: %d, 횟수: %d, 무게: %d\n",
                i + 1,
                temp->exercise[i].ExerciseName,
                temp->exercise[i].SetCount,
                temp->exercise[i].Count,
                temp->exercise[i].Weight);
        }
        printf("시간: %d\n\n", temp->Time);
        temp = temp->next;
    }
    printf("=========================================================================================================\n");
}
void RecordManagement(Node** head) {
    char date[7];
    int time;

    while (1) {
        int num = 10;
        printf("********운동 기록 관리********\n");
        printf("1. 운동 기록 작성\n");
        printf("2. 운동 기록 보기\n");
        printf("0. 뒤로 가기\n");
        printf("******************************\n");

        printf("메뉴를 선택하세요: ");
        scanf("%d", &num);
        getchar();

        system("cls");
        switch (num) {
        case 0:
            return;
        case 1:
            while (1) {
                printf("=========================================================================================================\n");
                printf("날짜를 입력하세요 (형식: YYMMDD): ");
                scanf("%6s", date);  // 6자만 읽도록 제한
                getchar();

                printf("운동 시간을 입력하세요 (분): ");
                scanf("%d", &time);
                getchar();

                Exercise exercises[10];
                InitExercises(exercises);
                GetExercises(exercises);

                AddExRecord(head, date, exercises, time);

                printf("=========================================================================================================\n");
                printf("기록이 완료되었습니다.\n");
                printf("계속 입력하시겠습니까? (Y/N): ");
                char cont;

                getchar();
                scanf("%c", &cont);
                getchar();
                if (cont != 'y' && cont != 'Y') {
                    system("cls");
                    break;
                }
                system("cls");
            }
            break;
        case 2:
            printRecords(*head);
            break;
        default:
            printf("잘못된 선택입니다. 다시 시도하세요.\n");
            break;
        }
    }
}
time_t convertToDate(char* date) {
    struct tm tm = { 0 };
    int year, month, day;
    if (sscanf(date, "%2d%2d%2d", &year, &month, &day) == 3) {
        tm.tm_year = year + 100;  // 2000년 기준 (1900 + 100 + 입력된 년도)
        tm.tm_mon = month - 1;    // 월은 0부터 시작
        tm.tm_mday = day;
        time_t t = mktime(&tm);
        if (t != -1) {
            return t;
        }
    }
    printf("날짜 변환 실패: 입력된 날짜가 유효하지 않습니다. [%s]\n", date);
    return (time_t)-1;
}
void addExerciseStats(Node* node, int* totalSets, int* totalCount, int* totalWeight, int* totalVolume) {
    for (int i = 0; i < 10 && node->exercise[i].ExerciseName[0] != '\0'; i++) {
        *totalSets += node->exercise[i].SetCount;
        *totalCount += node->exercise[i].Count;
        *totalWeight += node->exercise[i].Weight*node->exercise[i].SetCount*node->exercise[i].Count;
        *totalVolume += node->exercise[i].SetCount * node->exercise[i].Count;
    }
}
Stats calculateSevendaysStats(Node* head) {
    Stats stats = { 0, 0, 0, 0, 0 };
    if (head==NULL) {
        return stats;
    }

    Node* lastNode = head;
    while (lastNode->next) {
        lastNode = lastNode->next;
    }

    time_t lastDate = convertToDate(lastNode->Date);
    time_t sevenDaysAgo = lastDate - (7 * 24 * 60 * 60);

    for (Node* temp = head; temp; temp = temp->next) {
        time_t nodeDate = convertToDate(temp->Date);
        if (difftime(nodeDate, sevenDaysAgo) >= 0) {
            stats.totalTime += temp->Time;

            addExerciseStats(temp, &stats.totalSets, &stats.totalCount, &stats.totalWeight, &stats.totalVolume);
        }
    }

    return stats;
}void printSevendaysStats(Stats stats) {
    int num;

    printf("============================================최근 7일 운동 통계===========================================\n");
    printf("총 운동 시간: %d분\n", stats.totalTime);
    printf("총 세트 수: %d\n", stats.totalSets);
    printf("총 횟수: %d\n", stats.totalCount);
    printf("총 운동 볼륨: %d (세트 x 횟수)\n", stats.totalVolume);
    printf("총 무게: %dkg\n", stats.totalWeight);
    printf("=========================================================================================================\n");

    printf("뒤로 돌아가려면 아무 숫자나 입력해 주세요\n");
    scanf("%d", &num);

    return;

}
void sevendaysStats(Node* head) {
    int num = 10;
    if (head==NULL) {
        printf("============================================최근 7일 운동 통계===========================================\n");
        printf("운동 기록이 없습니다.\n");
        printf("=========================================================================================================\n");
        printf("뒤로 돌아가려면 아무 숫자나 입력해 주세요\n");
        scanf("%d", &num);
        return;
    }

    Stats stats = calculateSevendaysStats(head);
    printSevendaysStats(stats);
}
void GoalManagement(Stats stats) {
    int num = 10;

    while (1) {

        printf("*****************************\n");
        printf("1. 초기 체중 설정\n");
        printf("2. 현재 상태 업데이트\n");
        printf("3. 목표 설정\n");
        printf("4. 목표 달성도 확인\n");
        printf("0. 뒤로 가기\n");
        printf("*****************************\n");
        printf("메뉴를 선택하세요: ");
        scanf("%d", &num);
        system("cls");

        switch (num) {
        case 0:
            return;
        case 1:
            printf("초기 체중을 입력해주세요 (kg): ");
            scanf("%d", &Initweight);
            system("cls");
            break;
        case 2:
            printf("현재 체중을 입력해주세요 (kg): ");
            scanf("%d", &Pweight);
            system("cls");
            break;
        case 3:
            printf("목표 체중을 정수로 입력해주세요 (kg): ");
            scanf("%d", &Tweight);
            printf("목표 주별 운동 시간을 정수로 입력해주세요 (분): ");
            scanf("%d", &THealthTime);
            break;
        case 4:
            if (Pweight != 0 && Tweight != 0 && Initweight != 0 && THealthTime != 0) {
                printf("=========================================================================================================\n");
                printf("초기 체중 / 현재 체중 / 목표 체중: %dkg / %dkg / %dkg\n",Initweight, Pweight, Tweight);
                printf("현재 주별 운동시간 / 목표 주별 운동시간: %d분 / %d분\n", stats.totalTime, THealthTime);

                printf("현재 체중 목표 달성도\n");
                for (int i = 0; i < 100 - abs(Pweight - Tweight) * 100 / abs(Initweight - Tweight); i++) {
                    printf("#");
                    if (i > 100)
                        break;
                }
                printf("  %d%% \n", 100 - abs(Pweight - Tweight) * 100 / abs(Initweight - Tweight));

                printf("현재 주별 운동 시간 목표 달성도\n");
                for (int i = 0; i < 100 - abs(stats.totalTime - THealthTime) * 100 / THealthTime; i++) {
                    printf("#");
                    if (i > 100)
                        break;
                }
                printf("  %d%% \n", 100 - (THealthTime - stats.totalTime) * 100 / THealthTime);
                printf("=========================================================================================================\n");
                printf("뒤로 돌아가려면 아무 숫자나 입력해 주세요\n");
                scanf("%d", &num);
                getchar();
                system("cls");
                break;
            }
            else {
                printf("먼저 체중 또는 목표를 설정 해주세요\n");
                break;
            }
        default:
            printf("잘못된 선택입니다. 다시 시도하세요.\n");
        }
    }
}
void MainDisplay() {
    system("cls");
    printf("*******My Health Record*******\n");
    printf("1. 운동 기록 관리\n");
    printf("2. 운동 목표 관리\n");
    printf("3. 운동 데이터 분석\n");
    printf("0. 종료\n");
    printf("******************************\n");
}
void freeRecords(Node* head) {
    Node* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}
int main() {
    system("mode con:cols=140 lines=40");
    SetConsoleTitle(TEXT("My Health Record"));

    Node* head = NULL;
    while (1) {
        int num = 10;
        MainDisplay();
        printf("메뉴를 선택하세요: ");
        scanf("%d", &num);
        system("cls");
        switch (num) {
        case 0:
            freeRecords(head);
            return 0;
        case 1:
            RecordManagement(&head);
            break;
        case 2:
        {
            Stats stats = calculateSevendaysStats(head);
            GoalManagement(stats);
            break;
        }
        case 3:
            sevendaysStats(head);
            break;
        default:
            printf("잘못된 선택입니다. 다시 시도하세요.\n");
        }
    }

    return 0;
}
