#define GO(F) (F())

void fuck_yourself() {
	*(int*)0 = 69;
}

int main() {
	GO(fuck_yourself);
}
