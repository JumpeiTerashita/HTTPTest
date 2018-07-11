#include <Windows.h>
#include <iostream>


#include <cpprest/http_client.h>


using namespace web;
using namespace web::http;
using namespace web::http::client;

pplx::task<void> Get()
{
	return pplx::create_task([]
	{
		//GET先
		http_client client(L"https://jsonplaceholder.typicode.com/posts/14");
		return client.request(methods::GET);
	}).then([](http_response response)
	{
		if (response.status_code() == status_codes::OK)
		{
			// レスポンスを文字列として取得後、標準出力する
			 auto body = response.extract_string();
			 std::wcout << body.get().c_str() << std::endl;

			// レスポンスをJSONとして解析する
			//return response.extract_json();
		}
	});
		//}).then([](json::value json)
	//{
	//	std::wcout << json[L"title"].as_string() << std::endl;
	//	std::wcout << L"\n\n" << std::endl;
	//	std::wcout << json[L"body"].as_string() << std::endl;

	//	//// タイトルだけを取得する
	//	//std::wcout << json[L"title"].as_string() << std::endl;
	//});
}

pplx::task < void > Post()
{
	return pplx::create_task([]
	{
		json::value postData;
		auto sex = utility::conversions::to_string_t("man");
		postData[L"user_info"][L"name"] = json::value::string(L"John Doe");
		postData[L"user_info"][L"sex"] = json::value::string(sex);
		postData[L"user_info"][L"age"] = json::value::number(23);

		// Test Server
		// json-server
		http_client client(L"http://localhost:3000/api");
		return client.request(methods::POST, L"", postData.serialize(), L"application/json");
	}).then([](http_response response)
	{

		auto resCode = response.status_code();
		if (resCode == status_codes::Created)
		{
			auto body = response.extract_string();
			std::wcout << body.get().c_str() << std::endl;
			// std::cout << response.extract_json() << std::endl;
			return;
		}
  });
}


int GetTest() {
	
	SetConsoleOutputCP(CP_UTF8);

	try
	{
		Get().wait();
	}
	catch (const std::exception &e)
	{
		std::cout << "Error = " << e.what() << std::endl;
	}

	getchar();
	return 0;
}

int PostTest() {
	// コマンドプロンプトの文字コードをUTF-8に設定する
	SetConsoleOutputCP(CP_UTF8);

	try
	{
		Post().wait();
		std::cout << "Post Success : check the server`s file" << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cout << "Error = " << e.what() << std::endl;
	}

	getchar();
	return 0;
}

int main()
{
	PostTest();
	return 0;
}