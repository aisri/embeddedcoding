namespace Test {

class Singleton {
private:
    Singleton();
    static Singleton object;
public:
    Singleton(Singleton&) = delete;
    Singleton(const Singleton&) = delete;
    Singleton(const Singleton&&) = delete;
    Singleton& operator=(Singleton&) = delete;
    Singleton& operator=(Singleton&&) = delete;
    static Singleton& instance() {
        return object;
    }
};

class CopyConstructor {
    private:
        int i;
        int j;

    public:
        CopyConstructor(CopyConstructor& obj)
            : i(obj.i)
            , j(obj.j) {};
};

class MoveConstructor {
    private:
        int i, j;
        public:
        MoveConstructor& operator==(MoveConstructor &obj) {
            if (this != &obj) {
                this->i = obj.i;
                this->j = obj.j;
            }
            return *this;
        }
};
};
