class StationNode {
	friend bool add_train();
	friend bool sale_train();
	friend bool query_train();
	friend bool delete_train();
	friend bool modify_train();
	friend bool query_ticket();
};

class TrainNode {
	friend bool add_train();
	friend bool sale_train();
	friend bool query_train();
	friend bool delete_train();
	friend bool modify_train();
	friend class StationNode;
};

struct StationKey {
	friend bool query_ticket();
};

struct TrainKey {
};

class User {
	friend bool Register();
	friend bool login();
	friend bool modify_profile();
	friend bool query_profile();
	friend bool modify_privilege();
};