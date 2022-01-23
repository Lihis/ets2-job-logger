#include <gtest/gtest.h>
#include <jobplugin/Position.h>
#include <jobplugin/IdName.h>
#include <jobplugin/Job.h>

TEST(JsonTest, Position_ToString_ReturnsCorrectData) {
    position_t pos;
    pos.x = 1.0;
    pos.y = 2.0;
    pos.z = 3.0;
    pos.heading = 0.25;

    ASSERT_EQ(pos.ToString(), R"({"heading":0.25,"x":1.0,"y":2.0,"z":3.0})");
}

TEST(JsonTest, IdName_ToJson_HasRequireKeys) {
    id_name_t id_name;
    id_name.id = "a";
    id_name.name = "b";

    Json::Value json = id_name.ToJson();
    ASSERT_TRUE(json.isMember("id"));
    ASSERT_TRUE(json.isMember("name"));
}

class JobTest : public testing::TestWithParam<bool> {
public:
    void SetUp() override {
        m_job.source.city.id = "source.city.id";
        m_job.source.company.id = "source.company.id";

        m_job.destination.city.id = "destination.city.id";
        m_job.destination.company.id = "destination.company.id";
    }
protected:
    job_t m_job;
};

TEST_P(JobTest, Job_ToJson_HasCorrectKeys) {
    bool isSpecial = GetParam();
    m_job.setSpecial(isSpecial);
    Json::Value json = m_job.ToJson();
    ASSERT_TRUE(json.isMember("source"));
    ASSERT_TRUE(json["source"].isMember("city"));
    ASSERT_TRUE(json["source"]["city"].isMember("id"));
    ASSERT_EQ(json["source"]["city"]["id"].asString(), std::string("source.city.id"));
    if(isSpecial) {
        ASSERT_FALSE(json["source"].isMember("company"));
    } else {
        ASSERT_TRUE(json["source"].isMember("company"));
        ASSERT_TRUE(json["source"]["company"].isMember("id"));
        ASSERT_EQ(json["source"]["company"]["id"].asString(), std::string("source.company.id"));
    }

    ASSERT_TRUE(json.isMember("destination"));
    ASSERT_TRUE(json["destination"].isMember("city"));
    ASSERT_TRUE(json["destination"]["city"].isMember("id"));
    ASSERT_EQ(json["destination"]["city"]["id"].asString(), std::string("destination.city.id"));
    if(isSpecial) {
        ASSERT_FALSE(json["destination"].isMember("company"));
    } else {
        ASSERT_TRUE(json["destination"].isMember("company"));
        ASSERT_TRUE(json["destination"]["company"].isMember("id"));
        ASSERT_EQ(json["destination"]["company"]["id"].asString(), std::string("destination.company.id"));
    }
}

INSTANTIATE_TEST_SUITE_P(JsonTest, JobTest, testing::Values(false, true));