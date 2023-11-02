#include <json.hpp>
#include <fmt/core.h>
#include <fmt/std.h>

#include "fast_glTF64.hpp"

namespace SM64_fast_glTF64 {
    class PrintWithTabs {
    public:
        virtual std::string data_to_string(std::string tabs = "") const {
            return tabs;
        }
        std::string to_string(std::string tabs = "") const {
            return fmt::format("{{\n{}\n{}}}", this->data_to_string(tabs + "\t"), tabs);
        }
        operator std::string() const {
            return this->to_string();
        }
    };

    class DMATableSettings : public PrintWithTabs {
    public:
        std::string path;
        std::string name;
        std::string struct_name;
        bool per_frame_dma;

        void read_from_json(const nlohmann::json &json_data) {
            if (json_data.find("Animation Path") != json_data.end())
                this->path = json_data["Animation Path"];
            if (json_data.find("Struct Name") != json_data.end())
                this->struct_name = json_data["Struct Name"];
            if (json_data.find("Per Frame DMA") != json_data.end())
                this->per_frame_dma = json_data["Per Frame DMA"];
        }

        virtual std::string data_to_string(std::string tabs = "") const {
            return fmt::format("\n{}Animation Path: {}, Struct Name: {}, Per Frame DMA: {}", tabs,
                            this->path, this->struct_name, this->per_frame_dma);
        }
    };

    class Settings : public PrintWithTabs {
    public:
        bool debug_prints;
        bool write_old_file;
        bool skip;

        // Animation Settings
        std::string dma_table_name;
        bool reads_translation = true;

        void read_from_json(const nlohmann::json &json_data) {
            if (json_data.find("Debug Prints") != json_data.end())
                this->debug_prints = json_data["Debug Prints"];
            if (json_data.find("Write .old files") != json_data.end())
                this->write_old_file = json_data["Write .old files"];
            if (json_data.find("Skip") != json_data.end())
                this->skip = json_data["Skip"];
            if (json_data.find("Never Apply Translation Flags") != json_data.end())
                this->reads_translation = json_data["Never Apply Translation Flags"];
            if (json_data.find("DMA Table Name") != json_data.end())
                this->dma_table_name = json_data["DMA Table Name"];
        }

        std::string data_to_string(std::string tabs = "") const {
            return fmt::format("{}Debug Prints: {},\n{}Write Old File: {}, Skip: {},\n{}Never Apply "
                            "Translation Flags: {}, DMA Table Name: \"{}\"",
                            tabs, this->debug_prints, tabs, this->write_old_file, this->skip, tabs,
                            this->reads_translation, this->dma_table_name);
        }
    };

    class ActorSettings : public PrintWithTabs {
    public:
        Settings general;
        std::map<std::string, Settings> animations;

        void read_from_json(const nlohmann::json &json_data) {
            general.read_from_json(json_data);

            if (json_data.find("Animations Settings") == json_data.end())
                return;
            nlohmann::json::object_t animations = json_data["Animations Settings"];
            for (auto const &x : animations) {
                Settings animation = this->general;
                animation.read_from_json(x.second);
                this->animations[x.first] = animation;
            }
        }

        std::string animations_settings_to_string(std::string tabs = "") const {
            std::string result = fmt::format("{{\n");
            for (auto const &x : this->animations)
                result += fmt::format("{}{}: {}\n", tabs + "\t", x.first, x.second.to_string(tabs + "\t"));
            return result + fmt::format("{}}}", tabs);
        }

        std::string data_to_string(std::string tabs = "") const {
            return fmt::format("{}General: {}\n{}Animations: {}", tabs, this->general.to_string(tabs), tabs,
                            this->animations_settings_to_string(tabs));
        }
    };

    class ConverterSettings : public PrintWithTabs {
    public:
        unsigned int thread_amount = 1;
        std::vector<std::string> searchable_directories = std::vector<std::string>({ "actors/" });
        ActorSettings base;
        std::map<std::string, ActorSettings> actors;
        std::map<std::string, DMATableSettings> dma_tables;

        void read_from_json(const nlohmann::json &json_data) {
            if (json_data.find("Max Thread Amount") != json_data.end())
                this->thread_amount = json_data["Max Thread Amount"];

            if (json_data.find("Searchable Directories") != json_data.end())
                this->searchable_directories =
                    (std::vector<std::string>) json_data["Searchable Directories"];

            if (json_data.find("Default Actor Settings") != json_data.end())
                this->base.read_from_json(json_data["Default Actor Settings"]);

            if (json_data.find("Actor Settings") != json_data.end()) {
                nlohmann::json::object_t actor_json_settings = json_data["Actor Settings"];
                for (auto const &x : actor_json_settings) {
                    ActorSettings actor = this->base;
                    actor.read_from_json(x.second);
                    this->actors[x.first] = actor;
                }
            }
            if (json_data.find("DMA Tables") != json_data.end()) {
                nlohmann::json::object_t table_json_settings = json_data["DMA Tables"];
                for (auto const &x : table_json_settings) {
                    DMATableSettings table;
                    table.name = x.first;
                    table.read_from_json(x.second);
                    this->dma_tables[x.first] = table;
                }
            }
        }

        std::string table_settings_to_string(std::string tabs = "") const {
            std::string result = fmt::format("{{\n");
            for (auto const &x : this->dma_tables)
                result += fmt::format("{}{}: {}\n", tabs + "\t", x.first, x.second.to_string(tabs + "\t"));
            return result + fmt::format("{}}}", tabs);
        }

        std::string actor_settings_to_string(std::string tabs = "") const {
            std::string result = fmt::format("{{\n");
            for (auto const &x : this->actors)
                result += fmt::format("{}{}: {}\n", tabs + "\t", x.first, x.second.to_string(tabs + "\t"));
            return result + fmt::format("{}}}", tabs);
        }

        std::string data_to_string(std::string tabs = "") const {
            return fmt::format("{}Max Thread Amount: {}\n"
                            "{}Searchable Directories: {{ \"{}\" }}\n"
                            "{}Defaults: {}\n"
                            "{}Actor Settings: {}\n"
                            "{}DMA Tables Settings: {}\n",
                            tabs, this->thread_amount, tabs,
                            fmt::join(this->searchable_directories, "\", \""), tabs,
                            this->base.to_string(tabs), tabs, this->actor_settings_to_string(tabs), tabs,
                            this->table_settings_to_string(tabs));
        }
    };
}
SM64_fast_glTF64::ConverterSettings read_c_settings_file(std::filesystem::path path);