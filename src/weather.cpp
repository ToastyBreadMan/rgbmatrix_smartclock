#include "../include/weather.h"

namespace
{
    std::size_t callback(
            const char* in,
            std::size_t size,
            std::size_t num,
            std::string* out)
    {
        const std::size_t totalBytes(size * num);
        out->append(in, totalBytes);
        return totalBytes;
    }
}

Weather::Weather(std::string city_id, std::string api_key)
{
	this->city_id = city_id;
	this->api_key = api_key;

	this->weather_id = -1;
	this->cur_temp = -1;
	this->min_temp = -1;
	this->max_temp = -1;
}

void updateError(std::string object, nlohmann::json &j)
{
	fprintf(stderr, "ERROR: Could not find %s in weather api JSON response\n", object.c_str());
	fprintf(stderr, "-BEGIN JSON DUMP-\n%s\n--END JSON DUMP--\n", j.dump(4).c_str());
}

bool Weather::update() 
{
    long httpCode;
	std::string url;
    std::unique_ptr<std::string> httpData(new std::string());

    httpCode = 0;
	//url = "api.openweathermap.org/data/2.5/weather?id="
	//	+ std::string(this->city_id) + "&appid=" + std::string(this->api_key);

	url = "samples.openweathermap.org/data/2.5/weather?id=2172797&appid=b6907d289e10d714a6e88b30761fae22";

	printf("--> %s\n", url.c_str());

	CURL *curl = curl_easy_init();

    // Set remote URL.
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    // Don't bother trying IPv6, which would increase DNS resolution time.
    curl_easy_setopt(curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);

    // Don't wait forever, time out after 5 seconds.
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5);

    // Follow HTTP redirects if necessary.
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);


    // Hook up data handling function.
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);

    // Hook up data container (will be passed as the last parameter to the
    // callback handling function).  Can be any pointer type, since it will
    // internally be passed as a void pointer.
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, httpData.get());

    // Run our HTTP GET command, capture the HTTP response code, and clean up.
    curl_easy_perform(curl);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);                                                
    curl_easy_cleanup(curl);

	if (httpCode == 200) {
		nlohmann::json j = nlohmann::json::parse(*httpData.get());

		auto weather_it = j.find("weather");
		if (weather_it == j.end()) {
			updateError("weather", j);
			return false;
		}
		else {
			auto id_it = (*weather_it)[0].find("id");

			if (id_it == (*weather_it)[0].end()) {
				updateError("id", j);
				return false;
			}
			else {
                this->weather_id = id_it->get<int>();
			}
		}

        auto main_it = j.find("main");
        if (main_it == j.end()) {
			updateError("main", j);
			return false;
        }
        else {

            auto item_it = main_it->find("temp");
            if (item_it == main_it->end()) {
				updateError("temp", j);
				return false;
            }
            else {
                this->cur_temp = item_it->get<int>();
            }

			item_it = main_it->find("temp_min");
            if (item_it == main_it->end()) {
				updateError("temp_min", j);
				return false;
            }
            else {
                this->min_temp = item_it->get<int>();
            }

			item_it = main_it->find("temp_max");
            if (item_it == main_it->end()) {
				updateError("temp_max", j);
				return false;
            }
            else {
                this->max_temp = item_it->get<int>();
            }

        }

	}
	else {
		fprintf(stderr, "Error: Could not get data from %s\n", url.c_str());
		fprintf(stderr, "\tReturn code: %lld\n", httpCode);
		return false;
	}


	return true;
}

bool Weather::draw() {
	printf("id=%d\ntemp=%d\nmax-temp=%d\nmin-temp=%d\n", 
		this->weather_id, this->cur_temp, this->max_temp, this->min_temp);
	return true;
}
