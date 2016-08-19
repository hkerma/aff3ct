#ifndef SC_ENCODER_HPP_
#define SC_ENCODER_HPP_

#ifdef SYSTEMC
#include <vector>
#include <string>
#include <systemc>
#include <tlm>
#include <tlm_utils/simple_target_socket.h>
#include <tlm_utils/simple_initiator_socket.h>

#include "../Tools/Frame_trace/Frame_trace.hpp"
#include "../Tools/MIPP/mipp.h"

template <typename B>
class SC_Encoder : public sc_core::sc_module, public Encoder_interface<B>
{
	SC_HAS_PROCESS(SC_Encoder);

public:
	tlm_utils::simple_target_socket   <SC_Encoder> socket_in;
	tlm_utils::simple_initiator_socket<SC_Encoder> socket_out;

public:
	SC_Encoder(const int K, const int N, const int n_frames = 1, const sc_core::sc_module_name name = "SC_Encoder")
	: sc_module(name), Encoder_interface<B>(K, N, n_frames), socket_in("socket_in_SC_Encoder"), socket_out("socket_out_SC_Encoder")
	{
		socket_in.register_b_transport(this, &SC_Encoder::b_transport);
	};

	virtual ~SC_Encoder() {};

	virtual void encode(const mipp::vector<B>& U_K, mipp::vector<B>& X_N) = 0;

private:
	void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
	{
		const B* buffer_in = (B*)trans.get_data_ptr();
		int length = trans.get_data_length() / sizeof(B);
		
		assert(length == this->K * this->n_frames);

		mipp::vector<B> U_K(this->K * this->n_frames);
		mipp::vector<B> X_N(this->N * this->n_frames);

		std::copy(buffer_in, buffer_in + length, U_K.begin());

		this->encode(U_K, X_N);

		tlm::tlm_generic_payload payload;
		payload.set_data_ptr((unsigned char*)X_N.data());
		payload.set_data_length(X_N.size() * sizeof(B));

		sc_core::sc_time zero_time(sc_core::SC_ZERO_TIME);
		socket_out->b_transport(payload, zero_time);
	}
};

template <typename B>
using Encoder = SC_Encoder<B>;
#else
template <typename B>
using Encoder = Encoder_interface<B>;
#endif

#endif /* SC_ENCODER_HPP_ */
