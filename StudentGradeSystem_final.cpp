#include <iostream>
#include <string>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <cctype>
#include <iomanip>
#include <sstream>
using namespace std;
using namespace chrono;

const int MAX = 100000;

long long g_comparisons = 0;
long long g_swaps       = 0;

struct Student {
    int    id;
    string name;
    double gpa;
    int    age;
    string major;
};

Student students[MAX];
int n = 0;

const int    NUM_MAJORS = 10;
const string MAJORS[NUM_MAJORS] = {
    "Computer Science",
    "Electrical Engineering",
    "Mechanical Engineering",
    "Civil Engineering",
    "Information Technology",
    "Mathematics",
    "Physics",
    "Biology",
    "Chemistry",
    "Architecture"
};

void swapStudents(Student& a, Student& b) {
    Student t = a; a = b; b = t;
    g_swaps++;
}

void copyArray(Student src[], Student dst[], int sz) {
    for (int i = 0; i < sz; i++) dst[i] = src[i];
}

void resetCounters() { g_comparisons = 0; g_swaps = 0; }

string dbl(double v, int dec = 2) {
    ostringstream ss;
    ss << fixed << setprecision(dec) << v;
    return ss.str();
}

string pad(const string& s, int w) {
    if ((int)s.size() >= w) return s.substr(0, w - 1) + ">";
    return s + string(w - s.size(), ' ');
}

void clrscr() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pause() {
    cout << "\n  Press Enter to continue...";
    cin.get();
}

void divider(char c = '-', int w = 50) {
    cout << "  ";
    for (int i = 0; i < w; i++) cout << c;
    cout << "\n";
}

void header(const string& t) {
    cout << "\n";
    divider('=', 52);
    cout << "  " << t << "\n";
    divider('=', 52);
}

void boxTop(int w = 52) {
    cout << "  +";
    for (int i = 0; i < w; i++) cout << "-";
    cout << "+\n";
}

void boxBot(int w = 52) { boxTop(w); }

void boxRow(const string& label, const string& val,
            const string& col = "", int w = 52) {
    string line = " " + label + val;
    while ((int)line.size() < w) line += " ";
    cout << "  |" << col << line << "\033[0m|\n";
}

void barChart(const string& lbl, int val, int total,
              int barW, const string& col) {
    int filled = (total > 0) ? val * barW / total : 0;
    cout << "  " << pad(lbl, 12) << " [";
    cout << col;
    for (int i = 0; i < filled; i++) cout << "#";
    cout << "\033[0m";
    for (int i = filled; i < barW; i++) cout << ".";
    cout << "] " << val << "\n";
}

void printStudentRow(const Student& s) {
    string gpaStr = dbl(s.gpa);
    string gpaCol = (s.gpa >= 3.5) ? "\033[32m" : (s.gpa < 2.0) ? "\033[31m" : "\033[33m";
    cout << "  | "
         << pad(to_string(s.id), 9) << "| "
         << pad(s.name, 22)         << "| "
         << gpaCol << pad(gpaStr, 6) << "\033[0m| "
         << pad(to_string(s.age), 5) << "| "
         << pad(s.major, 26)         << "|\n";
}

void tableHeader() {
    cout << "\033[36m";
    cout << "  +---------+----------------------+------+-----+--------------------------+\n";
    cout << "  | ID      | Name                 | GPA  | Age | Major                    |\n";
    cout << "  +---------+----------------------+------+-----+--------------------------+\n";
    cout << "\033[0m";
}

void tableFooter() {
    cout << "\033[36m";
    cout << "  +---------+----------------------+------+-----+--------------------------+\n";
    cout << "\033[0m";
}

void bubbleSortGPA    (Student arr[], int size);
void insertionSortID  (Student arr[], int size);
void selectionSortAge (Student arr[], int size);
void mergeSortName    (Student arr[], int left, int right);
void quickSortGPA     (Student arr[], int low,  int high);
void generateRandom       (Student arr[], int size);
void generateNearlySorted (Student arr[], int size);
void generateReverseSorted(Student arr[], int size);

bool idExists(int id) {
    for (int i = 0; i < n; i++)
        if (students[i].id == id) return true;
    return false;
}

bool validName(const string& s) {
    bool hasLetter = false;
    for (char c : s) {
        if (!isalpha((unsigned char)c) && c != ' ') return false;
        if (isalpha((unsigned char)c)) hasLetter = true;
    }
    return hasLetter;
}

string chooseMajor() {
    cout << "\n";
    divider('-', 36);
    cout << "  SELECT MAJOR\n";
    divider('-', 36);
    for (int i = 0; i < NUM_MAJORS; i++)
        cout << "  \033[33m" << setw(2) << (i + 1) << "\033[0m. " << MAJORS[i] << "\n";
    divider('-', 36);
    cout << "  Choice (1-" << NUM_MAJORS << "): ";
    int ch;
    while (true) {
        if (cin >> ch && ch >= 1 && ch <= NUM_MAJORS) {
            cin.ignore(1000, '\n');
            return MAJORS[ch - 1];
        }
        cout << "\033[31m  Bad input. Try again: \033[0m";
        cin.clear(); cin.ignore(1000, '\n');
    }
}

void addStudent() {
    header("ADD STUDENT");
    if (n >= MAX) { cout << "\033[31m  Storage full.\033[0m\n"; return; }

    Student s;

    while (true) {
        cout << "  ID   : ";
        if (cin >> s.id && s.id > 0) {
            if (!idExists(s.id)) break;
            cout << "\033[31m  ID already exists.\033[0m\n";
        } else {
            cout << "\033[31m  Must be a positive number.\033[0m\n";
            cin.clear();
        }
        cin.ignore(1000, '\n');
    }
    cin.ignore(1000, '\n');

    while (true) {
        cout << "  Name : ";
        getline(cin, s.name);
        if (validName(s.name)) break;
        cout << "\033[31m  Letters and spaces only.\033[0m\n";
    }

    while (true) {
        cout << "  GPA  : ";
        if (cin >> s.gpa && s.gpa >= 0.0 && s.gpa <= 4.0) break;
        cout << "\033[31m  Enter 0.0 to 4.0.\033[0m\n";
        cin.clear(); cin.ignore(1000, '\n');
    }

    while (true) {
        cout << "  Age  : ";
        if (cin >> s.age && s.age >= 16 && s.age <= 100) break;
        cout << "\033[31m  Enter 16 to 100.\033[0m\n";
        cin.clear(); cin.ignore(1000, '\n');
    }

    s.major = chooseMajor();
    students[n++] = s;

    cout << "\n";
    divider('=', 52);
    cout << "\033[32m  SUCCESS: " << s.name << " added.\033[0m\n";
    divider('=', 52);
}

void deleteStudent() {
    header("DELETE STUDENT");
    int id;
    cout << "  Enter ID: ";
    while (!(cin >> id)) { cout << "\033[31m  Bad input: \033[0m"; cin.clear(); cin.ignore(1000,'\n'); }
    cin.ignore(1000, '\n');

    for (int i = 0; i < n; i++) {
        if (students[i].id == id) {
            cout << "\n  Deleting:\n";
            tableHeader();
            printStudentRow(students[i]);
            tableFooter();
            for (int j = i; j < n - 1; j++) students[j] = students[j + 1];
            n--;
            cout << "\033[32m  SUCCESS: Deleted.\033[0m\n";
            return;
        }
    }
    cout << "\033[31m  ID " << id << " not found.\033[0m\n";
}

void updateStudent() {
    header("UPDATE STUDENT");
    int id;
    cout << "  Enter ID: ";
    while (!(cin >> id)) { cout << "\033[31m  Bad input: \033[0m"; cin.clear(); cin.ignore(1000,'\n'); }
    cin.ignore(1000, '\n');

    for (int i = 0; i < n; i++) {
        if (students[i].id == id) {
            cout << "\n  Current:\n";
            tableHeader();
            printStudentRow(students[i]);
            tableFooter();

            cout << "\n  \033[33m1\033[0m. Update GPA\n";
            cout << "  \033[33m2\033[0m. Update Major\n";
            cout << "  \033[33m3\033[0m. Update Both\n";
            cout << "  Choice: ";
            int ch; cin >> ch; cin.ignore(1000, '\n');

            if (ch == 1 || ch == 3) {
                while (true) {
                    cout << "  New GPA: ";
                    if (cin >> students[i].gpa && students[i].gpa >= 0.0 && students[i].gpa <= 4.0)
                    { cin.ignore(1000,'\n'); break; }
                    cout << "\033[31m  Must be 0.0-4.0.\033[0m\n";
                    cin.clear(); cin.ignore(1000,'\n');
                }
            }
            if (ch == 2 || ch == 3) students[i].major = chooseMajor();

            cout << "\033[32m  SUCCESS: Updated.\033[0m\n";
            return;
        }
    }
    cout << "\033[31m  ID " << id << " not found.\033[0m\n";
}

void displayStudents() {
    header("ALL STUDENTS");
    if (n == 0) { cout << "\033[31m  No records.\033[0m\n"; return; }
    cout << "  Total: \033[1m" << n << "\033[0m\n\n";
    tableHeader();
    for (int i = 0; i < n; i++) printStudentRow(students[i]);
    tableFooter();
}

void bubbleSortGPA(Student arr[], int size) {
    for (int i = 0; i < size - 1; i++) {
        bool sw = false;
        for (int j = 0; j < size - i - 1; j++) {
            g_comparisons++;
            if (arr[j].gpa < arr[j+1].gpa) { swapStudents(arr[j], arr[j+1]); sw = true; }
        }
        if (!sw) break;
    }
}

void insertionSortID(Student arr[], int size) {
    for (int i = 1; i < size; i++) {
        Student key = arr[i]; int j = i - 1;
        while (j >= 0 && (g_comparisons++, arr[j].id > key.id))
        { arr[j+1] = arr[j]; g_swaps++; j--; }
        arr[j+1] = key;
    }
}

void selectionSortAge(Student arr[], int size) {
    for (int i = 0; i < size - 1; i++) {
        int m = i;
        for (int j = i+1; j < size; j++) { g_comparisons++; if (arr[j].age < arr[m].age) m = j; }
        if (m != i) swapStudents(arr[i], arr[m]);
    }
}

void mergeByName(Student arr[], int l, int mid, int r) {
    int n1 = mid-l+1, n2 = r-mid;
    Student* L = new Student[n1]; Student* R = new Student[n2];
    for (int i = 0; i < n1; i++) L[i] = arr[l+i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid+1+j];
    int i=0, j=0, k=l;
    while (i<n1 && j<n2) { g_comparisons++; arr[k++] = (L[i].name<=R[j].name) ? L[i++] : R[j++]; }
    while (i<n1) arr[k++]=L[i++];
    while (j<n2) arr[k++]=R[j++];
    delete[] L; delete[] R;
}

void mergeSortName(Student arr[], int l, int r) {
    if (l < r) { int m=l+(r-l)/2; mergeSortName(arr,l,m); mergeSortName(arr,m+1,r); mergeByName(arr,l,m,r); }
}

int medianOf3(Student arr[], int lo, int hi) {
    int mid=lo+(hi-lo)/2;
    if (arr[lo].gpa < arr[mid].gpa) swapStudents(arr[lo],arr[mid]);
    if (arr[lo].gpa < arr[hi].gpa)  swapStudents(arr[lo],arr[hi]);
    if (arr[mid].gpa < arr[hi].gpa) swapStudents(arr[mid],arr[hi]);
    swapStudents(arr[mid], arr[hi-1]);
    return hi-1;
}

int partitionGPA(Student arr[], int lo, int hi) {
    double piv = (hi-lo < 3) ? arr[hi].gpa : arr[medianOf3(arr,lo,hi)].gpa;
    int i = lo-1;
    for (int j=lo; j<hi; j++) { g_comparisons++; if (arr[j].gpa > piv) { i++; swapStudents(arr[i],arr[j]); } }
    swapStudents(arr[i+1], arr[hi]);
    return i+1;
}

void quickSortGPA(Student arr[], int lo, int hi) {
    if (lo < hi) { int p=partitionGPA(arr,lo,hi); quickSortGPA(arr,lo,p-1); quickSortGPA(arr,p+1,hi); }
}

int linearSearchName(const string& name) {
    for (int i = 0; i < n; i++) { g_comparisons++; if (students[i].name == name) return i; }
    return -1;
}

int linearSearchPartial(const string& q) {
    string ql = q;
    for (char& c : ql) c = (char)tolower((unsigned char)c);
    for (int i = 0; i < n; i++) {
        string nm = students[i].name;
        for (char& c : nm) c = (char)tolower((unsigned char)c);
        if (nm.find(ql) != string::npos) return i;
    }
    return -1;
}

int binarySearchID(Student arr[], int sz, int id) {
    int lo=0, hi=sz-1;
    while (lo<=hi) {
        int mid=lo+(hi-lo)/2; g_comparisons++;
        if (arr[mid].id==id) return mid;
        else if (arr[mid].id<id) lo=mid+1;
        else hi=mid-1;
    }
    return -1;
}

void searchMenu() {
    header("SEARCH STUDENTS");
    cout << "  \033[33m1\033[0m. By exact name   -- Linear Search   O(n)\n";
    cout << "  \033[33m2\033[0m. By ID           -- Binary Search   O(log n)\n";
    cout << "  \033[33m3\033[0m. Partial name    -- Case-insensitive scan\n";
    cout << "\n  Choice: ";
    int ch; cin >> ch; cin.ignore(1000,'\n');

    if (ch == 1) {
        cout << "  Name: "; string name; getline(cin, name);
        resetCounters();
        auto t0 = high_resolution_clock::now();
        int idx = linearSearchName(name);
        double ms = duration<double,milli>(high_resolution_clock::now()-t0).count();
        if (idx != -1) {
            cout << "\033[32m\n  FOUND:\033[0m\n";
            tableHeader(); printStudentRow(students[idx]); tableFooter();
        } else cout << "\033[31m  Not found after " << n << " checks.\033[0m\n";
        cout << "  Comparisons: " << g_comparisons << "   Time: " << ms << " ms\n";

    } else if (ch == 2) {
        cout << "  ID: "; int id; cin >> id; cin.ignore(1000,'\n');
        Student* tmp = new Student[n];
        copyArray(students, tmp, n);
        resetCounters(); insertionSortID(tmp, n);
        resetCounters();
        auto t0 = high_resolution_clock::now();
        int found = binarySearchID(tmp, n, id);
        double ms = duration<double,milli>(high_resolution_clock::now()-t0).count();
        if (found != -1) {
            cout << "\033[32m\n  FOUND:\033[0m\n";
            tableHeader(); printStudentRow(tmp[found]); tableFooter();
        } else cout << "\033[31m  Not found.\033[0m\n";
        int maxS=0; for(int t=n;t>0;t/=2) maxS++;
        cout << "  Comparisons: " << g_comparisons
             << "   Max possible: " << maxS
             << "   Time: " << ms << " ms\n";
        delete[] tmp;

    } else if (ch == 3) {
        cout << "  Partial name: "; string q; getline(cin, q);
        resetCounters();
        auto t0 = high_resolution_clock::now();
        int idx = linearSearchPartial(q);
        double ms = duration<double,milli>(high_resolution_clock::now()-t0).count();
        if (idx != -1) {
            cout << "\033[32m\n  FOUND (first match):\033[0m\n";
            tableHeader(); printStudentRow(students[idx]); tableFooter();
        } else cout << "\033[31m  Not found.\033[0m\n";
        cout << "  Time: " << ms << " ms\n";
    } else {
        cout << "\033[31m  Invalid.\033[0m\n";
    }
}

void statistics() {
    header("STATISTICS & DISTRIBUTION");
    if (n == 0) { cout << "  No data.\n"; return; }

    double sum=0, mx=students[0].gpa, mn=students[0].gpa;
    for (int i=0;i<n;i++) {
        sum+=students[i].gpa;
        if(students[i].gpa>mx) mx=students[i].gpa;
        if(students[i].gpa<mn) mn=students[i].gpa;
    }
    double mean=sum/n, var=0;
    for (int i=0;i<n;i++) var+=(students[i].gpa-mean)*(students[i].gpa-mean);
    double stddev=sqrt(var/n);

    Student* tmp=new Student[n]; copyArray(students,tmp,n);
    quickSortGPA(tmp,0,n-1);
    double med=(n%2==0)?(tmp[n/2-1].gpa+tmp[n/2].gpa)/2.0:tmp[n/2].gpa;
    delete[] tmp;

    int dist[5]={0};
    for(int i=0;i<n;i++){
        double g=students[i].gpa;
        if(g<1.0)dist[0]++; else if(g<2.0)dist[1]++; else if(g<3.0)dist[2]++; else if(g<3.5)dist[3]++; else dist[4]++;
    }

    int mxCnt=0;
    for(int i=0;i<5;i++) if(dist[i]>mxCnt) mxCnt=dist[i];

    boxTop(52);
    boxRow("  Total      : ", to_string(n));
    boxRow("  Mean GPA   : ", dbl(mean), "\033[33m");
    boxRow("  Median GPA : ", dbl(med));
    boxRow("  Std Dev    : ", dbl(stddev));
    boxRow("  Highest    : ", dbl(mx), "\033[32m");
    boxRow("  Lowest     : ", dbl(mn), "\033[31m");
    boxBot(52);

    cout << "\n  GPA DISTRIBUTION:\n\n";
    const char* lbl[]={"F  <1.0  ","D  1-1.99","C  2-2.99","B  3-3.49","A  3.5-4 "};
    const char* col[]={"\033[31m","\033[33m","\033[34m","\033[32m","\033[32m"};
    for(int i=0;i<5;i++) barChart(lbl[i], dist[i], mxCnt, 28, col[i]);
}

void topK() {
    header("TOP-K RANKING");
    if(n==0){cout<<"  No records.\n";return;}
    int k; cout<<"  Enter K: "; cin>>k; cin.ignore(1000,'\n');
    if(k<=0||k>n){cout<<"\033[31m  Invalid K.\033[0m\n";return;}

    Student* tmp=new Student[n]; copyArray(students,tmp,n);
    quickSortGPA(tmp,0,n-1);

    cout<<"\n  Top "<<k<<" by GPA:\n\n";
    cout<<"\033[36m  +------+------+----------------------+-----+--------------------------+\033[0m\n";
    cout<<"\033[36m  | Rank | GPA  | Name                 | Age | Major                    |\033[0m\n";
    cout<<"\033[36m  +------+------+----------------------+-----+--------------------------+\033[0m\n";
    for(int i=0;i<k;i++){
        const Student& s=tmp[i];
        string rk="#"+to_string(i+1);
        string g=dbl(s.gpa);
        string gCol=(s.gpa>=3.5)?"\033[32m":"\033[33m";
        cout<<"  | "<<pad(rk,4)<<" | "<<gCol<<pad(g,4)<<"\033[0m | "
            <<pad(s.name,20)<<" | "<<pad(to_string(s.age),3)<<" | "<<pad(s.major,24)<<" |\n";
    }
    cout<<"\033[36m  +------+------+----------------------+-----+--------------------------+\033[0m\n";
    delete[] tmp;
}

void filterByMajor() {
    header("FILTER BY MAJOR");
    string major=chooseMajor();
    int count=0;
    tableHeader();
    for(int i=0;i<n;i++) if(students[i].major==major){printStudentRow(students[i]);count++;}
    tableFooter();
    if(count==0) cout<<"\033[31m  No students in that major.\033[0m\n";
    else         cout<<"\033[32m  Found: "<<count<<" student(s).\033[0m\n";
}

void detectDuplicates() {
    header("DUPLICATE GPA DETECTOR");
    if(n<2){cout<<"  Need at least 2 records.\n";return;}
    Student* tmp=new Student[n]; copyArray(students,tmp,n);
    quickSortGPA(tmp,0,n-1);
    bool found=false;
    for(int i=0;i<n-1;i++){
        if(tmp[i].gpa==tmp[i+1].gpa){
            cout<<"\033[33m  [DUP] GPA "<<dbl(tmp[i].gpa)
                <<"  ->  "<<tmp[i].name<<"  &  "<<tmp[i+1].name<<"\033[0m\n";
            found=true;
        }
    }
    if(!found) cout<<"\033[32m  No duplicate GPAs found.\033[0m\n";
    delete[] tmp;
}

struct AVLNode { Student data; AVLNode* L; AVLNode* R; int h; };
AVLNode* avlRoot=NULL;

int avlH(AVLNode* n){return n?n->h:0;}
int avlMx(int a,int b){return a>b?a:b;}
int avlBal(AVLNode* n){return n?avlH(n->L)-avlH(n->R):0;}

AVLNode* avlNew(Student s){AVLNode* nd=new AVLNode;nd->data=s;nd->L=nd->R=NULL;nd->h=1;return nd;}

AVLNode* avlRR(AVLNode* y){
    AVLNode* x=y->L,*T2=x->R; x->R=y; y->L=T2;
    y->h=avlMx(avlH(y->L),avlH(y->R))+1; x->h=avlMx(avlH(x->L),avlH(x->R))+1; return x;
}
AVLNode* avlLR(AVLNode* x){
    AVLNode* y=x->R,*T2=y->L; y->L=x; x->R=T2;
    x->h=avlMx(avlH(x->L),avlH(x->R))+1; y->h=avlMx(avlH(y->L),avlH(y->R))+1; return y;
}
AVLNode* avlIns(AVLNode* nd,Student s){
    if(!nd) return avlNew(s);
    if(s.id<nd->data.id) nd->L=avlIns(nd->L,s);
    else if(s.id>nd->data.id) nd->R=avlIns(nd->R,s);
    else return nd;
    nd->h=1+avlMx(avlH(nd->L),avlH(nd->R));
    int b=avlBal(nd);
    if(b>1&&s.id<nd->L->data.id) return avlRR(nd);
    if(b<-1&&s.id>nd->R->data.id) return avlLR(nd);
    if(b>1&&s.id>nd->L->data.id){nd->L=avlLR(nd->L);return avlRR(nd);}
    if(b<-1&&s.id<nd->R->data.id){nd->R=avlRR(nd->R);return avlLR(nd);}
    return nd;
}
AVLNode* avlSrch(AVLNode* nd,int id){
    if(!nd) return NULL;
    g_comparisons++;
    if(id==nd->data.id) return nd;
    return (id<nd->data.id)?avlSrch(nd->L,id):avlSrch(nd->R,id);
}
void avlIn(AVLNode* nd){
    if(nd){avlIn(nd->L);tableHeader();printStudentRow(nd->data);tableFooter();avlIn(nd->R);}
}

void avlMenu(){
    header("AVL TREE  (Self-Balancing BST)");
    cout<<"  \033[33m1\033[0m. Insert all students\n";
    cout<<"  \033[33m2\033[0m. Search by ID  [O(log n)]\n";
    cout<<"  \033[33m3\033[0m. Inorder display (sorted by ID)\n";
    cout<<"\n  Choice: "; int ch; cin>>ch; cin.ignore(1000,'\n');
    if(ch==1){
        avlRoot=NULL;
        for(int i=0;i<n;i++) avlRoot=avlIns(avlRoot,students[i]);
        cout<<"\033[32m  Inserted "<<n<<" students.\033[0m\n";
    } else if(ch==2){
        cout<<"  ID: "; int id; cin>>id; cin.ignore(1000,'\n');
        resetCounters();
        AVLNode* r=avlSrch(avlRoot,id);
        if(r){cout<<"\033[32m  FOUND:\033[0m\n";tableHeader();printStudentRow(r->data);tableFooter();}
        else cout<<"\033[31m  Not found.\033[0m\n";
        cout<<"  Comparisons: "<<g_comparisons<<"\n";
    } else if(ch==3){
        avlIn(avlRoot);
    }
}

void generateRandom(Student arr[], int size){
    srand((unsigned)time(0));
    const string nm[]={"Ali","Sara","Omar","Lina","Hassan","Nour","Yusuf","Dina","Khaled","Maya"};
    for(int i=0;i<size;i++){
        arr[i].id=i+1;
        arr[i].name=nm[rand()%10]+" "+to_string(i+1);
        arr[i].gpa=(rand()%401)/100.0;
        arr[i].age=18+rand()%10;
        arr[i].major=MAJORS[rand()%NUM_MAJORS];
    }
}

void generateNearlySorted(Student arr[], int size){
    generateRandom(arr,size); quickSortGPA(arr,0,size-1);
    int d=size/20;
    for(int i=0;i<d;i++){int a=rand()%size,b=rand()%size;swapStudents(arr[a],arr[b]);}
}

void generateReverseSorted(Student arr[], int size){
    generateRandom(arr,size); quickSortGPA(arr,0,size-1);
    for(int i=0,j=size-1;i<j;i++,j--) swapStudents(arr[i],arr[j]);
}

void sortMenu(){
    header("SORT STUDENTS");
    divider('-', 78);
    cout<<"  # | Algorithm      | Field    | Complexity             | Scenario\n";
    divider('-', 78);
    cout<<"  \033[33m1\033[0m | Bubble Sort    | GPA desc | O(n^2) / O(n) best     | Nearly-sorted data\n";
    cout<<"  \033[33m2\033[0m | Insertion Sort | ID  asc  | O(n^2) / O(n) best     | Small/incremental\n";
    cout<<"  \033[33m3\033[0m | Selection Sort | Age asc  | O(n^2) always          | Minimize writes\n";
    cout<<"  \033[33m4\033[0m | Merge Sort     | Name asc | O(n log n) guaranteed  | Large, stable sort\n";
    cout<<"  \033[33m5\033[0m | Quick Sort     | GPA desc | O(n log n) avg         | Large, fastest avg\n";
    divider('-', 78);
    cout<<"\n  Choice: "; int ch; cin>>ch; cin.ignore(1000,'\n');
    if(ch<1||ch>5){cout<<"\033[31m  Invalid.\033[0m\n";return;}

    const string desc[]={"Bubble Sort -- GPA desc","Insertion Sort -- ID asc",
        "Selection Sort -- Age asc","Merge Sort -- Name asc","Quick Sort -- GPA desc"};

    resetCounters();
    auto t0=high_resolution_clock::now();
    if(ch==1) bubbleSortGPA(students,n);
    else if(ch==2) insertionSortID(students,n);
    else if(ch==3) selectionSortAge(students,n);
    else if(ch==4) mergeSortName(students,0,n-1);
    else           quickSortGPA(students,0,n-1);
    double ms=duration<double,milli>(high_resolution_clock::now()-t0).count();

    divider('=', 52);
    cout<<"\033[32m  DONE: "<<desc[ch-1]<<"\033[0m\n";
    divider('-', 52);
    cout<<"  Comparisons : "<<g_comparisons<<"\n";
    cout<<"  Swaps       : "<<g_swaps<<"\n";
    cout<<"  Time        : "<<ms<<" ms\n";
    divider('=', 52);
}

void loadSampleData(){
    header("LOAD SAMPLE DATA");
    cout<<"  \033[33m1\033[0m.     50 records  (small)\n";
    cout<<"  \033[33m2\033[0m.  1,000 records  (medium)\n";
    cout<<"  \033[33m3\033[0m. 10,000 records  (large)\n";
    cout<<"  \033[33m4\033[0m. 100,000 records (benchmark)\n";
    cout<<"\n  Choice: "; int ch; cin>>ch; cin.ignore(1000,'\n');
    const int sz[]={50,1000,10000,100000};
    if(ch<1||ch>4){cout<<"\033[31m  Invalid.\033[0m\n";return;}
    generateRandom(students,sz[ch-1]); n=sz[ch-1];
    cout<<"\033[32m  Loaded "<<n<<" students.\033[0m\n";
}

struct BenchResult{string alg;int sz;string dt;double ms;long long cmp,sw;};
BenchResult benchResults[300];
int benchCount=0;

void runBenchmark(){
    header("BENCHMARKING ENGINE");
    cout<<"  5 algorithms x 4 sizes x 3 distributions = 60 runs\n";
    cout<<"  Sizes: 50 | 1,000 | 10,000 | 100,000\n";
    cout<<"  Types: Random | Nearly Sorted | Reverse Sorted\n\n";
    cout<<"  NOTE: O(n^2) algorithms skipped at n=100,000\n\n";
    cout<<"  Press Enter to start..."; cin.get();

    const int sz[]={50,1000,10000,100000};
    const string dt[]={"Random","Nearly Sorted","Reverse Sorted"};
    const string alg[]={"Bubble Sort","Insertion Sort","Selection Sort","Merge Sort","Quick Sort"};

    benchCount=0;

    for(int di=0;di<3;di++){
        cout<<"\n\033[36m  --- "<<dt[di]<<" ---\033[0m\n";
        for(int si=0;si<4;si++){
            int s=sz[si];
            Student* base=new Student[s]; Student* arr=new Student[s];
            if(di==0) generateRandom(base,s);
            else if(di==1) generateNearlySorted(base,s);
            else generateReverseSorted(base,s);

            for(int ai=0;ai<5;ai++){
                bool skip=(s==100000&&ai<3);
                copyArray(base,arr,s); resetCounters();
                double ms=-1;
                if(!skip){
                    auto t0=high_resolution_clock::now();
                    if(ai==0) bubbleSortGPA(arr,s);
                    else if(ai==1) insertionSortID(arr,s);
                    else if(ai==2) selectionSortAge(arr,s);
                    else if(ai==3) mergeSortName(arr,0,s-1);
                    else quickSortGPA(arr,0,s-1);
                    ms=duration<double,milli>(high_resolution_clock::now()-t0).count();
                }
                BenchResult& r=benchResults[benchCount++];
                r.alg=alg[ai]; r.sz=s; r.dt=dt[di]; r.ms=ms;
                r.cmp=skip?-1:g_comparisons; r.sw=skip?-1:g_swaps;

                cout<<"  "<<setw(16)<<left<<alg[ai]<<" n="<<setw(7)<<left<<s;
                if(skip) cout<<"\033[33m  SKIPPED\033[0m\n";
                else     cout<<"\033[32m  "<<ms<<" ms\033[0m\n";
            }
            delete[] base; delete[] arr;
        }
    }

    cout<<"\n\n";
    divider('=', 76);
    cout<<"  FULL RESULTS TABLE\n";
    divider('=', 76);
    cout<<"\033[36m"
        <<"  Algorithm        | Size     | Type           | Time(ms)   | Comparisons  | Swaps\n"
        <<"  -----------------+----------+----------------+------------+--------------+----------\n"
        <<"\033[0m";
    for(int i=0;i<benchCount;i++){
        BenchResult& r=benchResults[i];
        cout<<"  "<<setw(17)<<left<<r.alg
            <<"| "<<setw(9)<<left<<r.sz
            <<"| "<<setw(15)<<left<<r.dt
            <<"| "<<setw(11)<<left<<(r.ms<0?"SKIPPED":dbl(r.ms,3))
            <<"| "<<setw(13)<<left<<(r.cmp<0?"SKIPPED":to_string(r.cmp))
            <<"| "<<(r.sw<0?"SKIPPED":to_string(r.sw))<<"\n";
    }
    divider('=', 76);
}

void dashboard(){
    while(true){
        clrscr();

        double sum=0, mx=0, mn=4.0;
        int distArr[5]={0};
        int majorCount[NUM_MAJORS]={0};
        for(int i=0;i<n;i++){
            sum+=students[i].gpa;
            if(students[i].gpa>mx) mx=students[i].gpa;
            if(students[i].gpa<mn) mn=students[i].gpa;
            double g=students[i].gpa;
            if(g<1.0)distArr[0]++; else if(g<2.0)distArr[1]++;
            else if(g<3.0)distArr[2]++; else if(g<3.5)distArr[3]++; else distArr[4]++;
            for(int m=0;m<NUM_MAJORS;m++)
                if(students[i].major==MAJORS[m]){majorCount[m]++;break;}
        }
        double mean=(n>0)?sum/n:0;

        int mxDist=0;
        for(int i=0;i<5;i++) if(distArr[i]>mxDist) mxDist=distArr[i];
        int mxMaj=0;
        for(int i=0;i<NUM_MAJORS;i++) if(majorCount[i]>mxMaj) mxMaj=majorCount[i];

        cout<<"\033[1m\033[36m";
        divider('=', 66);
        cout<<"  DASHBOARD  --  STUDENT GRADE MANAGEMENT SYSTEM\n";
        divider('=', 66);
        cout<<"\033[0m";

        cout<<"\n\033[1m  OVERVIEW\033[0m\n";
        boxTop(34);
        boxRow("  Students    : ", to_string(n));
        boxRow("  Mean GPA    : ", dbl(mean),"\033[33m");
        boxRow("  Highest GPA : ", dbl(mx),"\033[32m");
        boxRow("  Lowest GPA  : ", dbl(mn),"\033[31m");
        boxBot(34);

        Student* tmp=new Student[n];
        copyArray(students,tmp,n);
        if(n>0) quickSortGPA(tmp,0,n-1);

        cout<<"\n\033[1m  TOP 5 STUDENTS\033[0m\n";
        cout<<"\033[36m  +------+------+--------------------+--------------------------+\033[0m\n";
        cout<<"\033[36m  | Rank | GPA  | Name               | Major                    |\033[0m\n";
        cout<<"\033[36m  +------+------+--------------------+--------------------------+\033[0m\n";
        for(int i=0;i<5&&i<n;i++){
            string rk="#"+to_string(i+1);
            cout<<"  | "<<pad(rk,4)<<" | "
                <<"\033[32m"<<pad(dbl(tmp[i].gpa),4)<<"\033[0m | "
                <<pad(tmp[i].name,18)<<" | "
                <<pad(tmp[i].major,24)<<" |\n";
        }
        cout<<"\033[36m  +------+------+--------------------+--------------------------+\033[0m\n";
        delete[] tmp;

        cout<<"\n\033[1m  GPA DISTRIBUTION\033[0m\n\n";
        const char* gl[]={"F  <1.0  ","D  1-1.99","C  2-2.99","B  3-3.49","A  3.5-4 "};
        const char* gc[]={"\033[31m","\033[33m","\033[34m","\033[32m","\033[32m"};
        for(int i=0;i<5;i++) barChart(gl[i],distArr[i],mxDist,26,gc[i]);

        cout<<"\n\033[1m  STUDENTS BY MAJOR\033[0m\n\n";
        for(int i=0;i<NUM_MAJORS;i++){
            if(majorCount[i]>0)
                barChart(MAJORS[i].substr(0,12),majorCount[i],mxMaj,20,"\033[36m");
        }

        cout<<"\n";
        divider('=', 52);
        cout<<"\033[33m  DASHBOARD ACTIONS\033[0m\n";
        divider('-', 52);
        cout<<"  \033[33mS\033[0m. Quick Search (partial name)\n";
        cout<<"  \033[33mT\033[0m. Show Top-K\n";
        cout<<"  \033[33mD\033[0m. Detect Duplicates\n";
        cout<<"  \033[33mR\033[0m. Refresh dashboard\n";
        cout<<"  \033[33mX\033[0m. Back to main menu\n";
        divider('=', 52);
        cout<<"  Action: ";

        string act; cin>>act;
        cin.ignore(1000,'\n');
        char a=(char)toupper((unsigned char)act[0]);

        if(a=='X') break;

        else if(a=='S'){
            cout<<"  Partial name: "; string q; getline(cin,q);
            if(q.empty()){cout<<"\033[31m  Empty query.\033[0m\n";}
            else{
                resetCounters();
                auto t0=high_resolution_clock::now();
                int idx=linearSearchPartial(q);
                double ms=duration<double,milli>(high_resolution_clock::now()-t0).count();
                if(idx!=-1){
                    cout<<"\033[32m  FOUND:\033[0m\n";
                    tableHeader(); printStudentRow(students[idx]); tableFooter();
                } else cout<<"\033[31m  No match.\033[0m\n";
                cout<<"  Time: "<<ms<<" ms\n";
            }
            pause();
        }

        else if(a=='T'){
            if(n==0){cout<<"\033[31m  No records.\033[0m\n";pause();continue;}
            int k; cout<<"  Enter K: "; cin>>k; cin.ignore(1000,'\n');
            if(k<=0||k>n){cout<<"\033[31m  Invalid K.\033[0m\n";pause();continue;}
            Student* t2=new Student[n]; copyArray(students,t2,n);
            quickSortGPA(t2,0,n-1);
            tableHeader();
            for(int i=0;i<k;i++) printStudentRow(t2[i]);
            tableFooter();
            delete[] t2;
            pause();
        }

        else if(a=='D'){
            if(n<2){cout<<"\033[31m  Need 2+ records.\033[0m\n";pause();continue;}
            Student* t2=new Student[n]; copyArray(students,t2,n);
            quickSortGPA(t2,0,n-1);
            bool found=false;
            for(int i=0;i<n-1;i++){
                if(t2[i].gpa==t2[i+1].gpa){
                    cout<<"\033[33m  [DUP] "<<dbl(t2[i].gpa)
                        <<"  ->  "<<t2[i].name<<"  &  "<<t2[i+1].name<<"\033[0m\n";
                    found=true;
                }
            }
            if(!found) cout<<"\033[32m  No duplicates.\033[0m\n";
            delete[] t2;
            pause();
        }

        else if(a=='R'){
            continue;
        }

        else {
            cout<<"\033[31m  Invalid action.\033[0m\n";
            pause();
        }
    }
}

void printMainMenu(){
    clrscr();
    cout<<"\n\033[1m\033[36m";
    cout<<"  +==========================================+\n";
    cout<<"  |   STUDENT GRADE MANAGEMENT SYSTEM       |\n";
    cout<<"  |   Design & Analysis of Algorithms       |\n";
    cout<<"  +==========================================+\033[0m\n";
    cout<<"  |  Students in memory: \033[1m"
        <<setw(5)<<left<<n<<"\033[0m                  |\n";
    cout<<"\033[36m  +==========================================+\033[0m\n\n";

    cout<<"\033[33m  RECORDS\033[0m\n";
    cout<<"  \033[32m 1\033[0m  Dashboard (interactive)\n";
    cout<<"  \033[32m 2\033[0m  Add Student\n";
    cout<<"  \033[32m 3\033[0m  Delete Student\n";
    cout<<"  \033[32m 4\033[0m  Update Student\n";
    cout<<"  \033[32m 5\033[0m  Display All Students\n";
    cout<<"\033[36m  ------------------------------------------\033[0m\n";

    cout<<"\033[33m  ALGORITHMS\033[0m\n";
    cout<<"  \033[32m 6\033[0m  Sort Students\n";
    cout<<"  \033[32m 7\033[0m  Search Students\n";
    cout<<"\033[36m  ------------------------------------------\033[0m\n";

    cout<<"\033[33m  ANALYTICS\033[0m\n";
    cout<<"  \033[32m 8\033[0m  Top-K Ranking\n";
    cout<<"  \033[32m 9\033[0m  Statistics & Distribution\n";
    cout<<"  \033[32m10\033[0m  Filter by Major\n";
    cout<<"  \033[32m11\033[0m  Detect Duplicate GPAs\n";
    cout<<"\033[36m  ------------------------------------------\033[0m\n";

    cout<<"\033[33m  ADVANCED\033[0m\n";
    cout<<"  \033[32m12\033[0m  AVL Tree Operations\n";
    cout<<"  \033[32m13\033[0m  Run Full Benchmark\n";
    cout<<"  \033[32m14\033[0m  Load Sample Data\n";
    cout<<"\033[36m  ------------------------------------------\033[0m\n";

    cout<<"  \033[31m 0\033[0m  Exit\n";
    cout<<"\033[36m  +==========================================+\033[0m\n";
    cout<<"\n  > ";
}

int main(){
    int choice;
    while(true){
        printMainMenu();
        if(!(cin>>choice)){cin.clear();cin.ignore(1000,'\n');continue;}
        cin.ignore(1000,'\n');
        switch(choice){
            case  1: dashboard();        break;
            case  2: addStudent();       pause(); break;
            case  3: deleteStudent();    pause(); break;
            case  4: updateStudent();    pause(); break;
            case  5: displayStudents();  pause(); break;
            case  6: sortMenu();         pause(); break;
            case  7: searchMenu();       pause(); break;
            case  8: topK();             pause(); break;
            case  9: statistics();       pause(); break;
            case 10: filterByMajor();    pause(); break;
            case 11: detectDuplicates(); pause(); break;
            case 12: avlMenu();          pause(); break;
            case 13: runBenchmark();     pause(); break;
            case 14: loadSampleData();   pause(); break;
            case  0: cout<<"\n\033[36m  Goodbye!\033[0m\n\n"; return 0;
            default: cout<<"\033[31m  Invalid choice.\033[0m\n"; pause();
        }
    }
}
