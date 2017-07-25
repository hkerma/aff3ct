#ifndef FACTORY_SIMULATION_MAIN_H_
#define FACTORY_SIMULATION_MAIN_H_

#include <string>

#include "../Factory.hpp"

namespace aff3ct
{
namespace launcher
{
class Launcher;
}
}

namespace aff3ct
{
namespace factory
{
struct Launcher : public Factory
{
	static const std::string name;
	static const std::string prefix;

	struct parameters
	{
		std::string cde_type;

		std::string sim_type        = "BFER";
		int         sim_prec        = 32;
		bool        display_help    = false;
		bool        display_version = false;
	};

	template <typename B = int, typename R = float, typename Q = R, typename QD = Q>
	static launcher::Launcher* build(const parameters &params, const int argc, const char **argv);

	static void build_args(arg_map &req_args, arg_map &opt_args, const std::string p = prefix);
	static void store_args(const arg_val_map &vals, parameters &params, const std::string p = prefix);
	static void make_header(params_list& head_sim, const parameters& params);

private:
	template <typename B = int, typename R = float, typename Q = R, typename QD = Q>
	static launcher::Launcher* build_exit(const parameters &params, const int argc, const char **argv);
};
}
}

#endif /* FACTORY_SIMULATION_MAIN_H_ */