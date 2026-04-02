class NCursesSession {
private:
  NCursesSession();

public:
  ~NCursesSession();

  NCursesSession(const NCursesSession&) = delete;
  NCursesSession(NCursesSession&&) = delete;
  auto operator=(const NCursesSession&) -> NCursesSession& = delete;
  auto operator=(NCursesSession&&) -> NCursesSession& = delete;

  static void init();
};
