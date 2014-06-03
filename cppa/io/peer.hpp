/******************************************************************************\
 *           ___        __                                                    *
 *          /\_ \    __/\ \                                                   *
 *          \//\ \  /\_\ \ \____    ___   _____   _____      __               *
 *            \ \ \ \/\ \ \ '__`\  /'___\/\ '__`\/\ '__`\  /'__`\             *
 *             \_\ \_\ \ \ \ \L\ \/\ \__/\ \ \L\ \ \ \L\ \/\ \L\.\_           *
 *             /\____\\ \_\ \_,__/\ \____\\ \ ,__/\ \ ,__/\ \__/.\_\          *
 *             \/____/ \/_/\/___/  \/____/ \ \ \/  \ \ \/  \/__/\/_/          *
 *                                          \ \_\   \ \_\                     *
 *                                           \/_/    \/_/                     *
 *                                                                            *
 * Copyright (C) 2011-2013                                                    *
 * Dominik Charousset <dominik.charousset@haw-hamburg.de>                     *
 *                                                                            *
 * This file is part of libcppa.                                              *
 * libcppa is free software: you can redistribute it and/or modify it under   *
 * the terms of the GNU Lesser General Public License as published by the     *
 * Free Software Foundation; either version 2.1 of the License,               *
 * or (at your option) any later version.                                     *
 *                                                                            *
 * libcppa is distributed in the hope that it will be useful,                 *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                       *
 * See the GNU Lesser General Public License for more details.                *
 *                                                                            *
 * You should have received a copy of the GNU Lesser General Public License   *
 * along with libcppa. If not, see <http://www.gnu.org/licenses/>.            *
\******************************************************************************/


#ifndef CPPA_peer_IMPL_HPP
#define CPPA_peer_IMPL_HPP

#include <map>
#include <cstdint>

#include "cppa/extend.hpp"
#include "cppa/node_id.hpp"
#include "cppa/actor_proxy.hpp"
#include "cppa/partial_function.hpp"
#include "cppa/type_lookup_table.hpp"
#include "cppa/weak_intrusive_ptr.hpp"

#include "cppa/util/buffer.hpp"

#include "cppa/io/input_stream.hpp"
#include "cppa/io/output_stream.hpp"
#include "cppa/io/buffered_writing.hpp"
#include "cppa/io/default_message_queue.hpp"

namespace cppa { namespace io {

class middleman_impl;

class peer : public continuable {

    friend class middleman_impl;

    typedef continuable super;

 public:

    virtual void enqueue(msg_hdr_cref hdr, const any_tuple& msg) = 0;

    inline bool stop_on_last_proxy_exited() const {
        return m_stop_on_last_proxy_exited;
    }

    inline const node_id& node() const {
        return *m_node;
    }

    inline bool has_node() const {
        return static_cast<bool>(m_node);
    }

    void set_node(node_id_ptr ptr) {
        m_node = std::move(ptr);
    }

    virtual bool has_unwritten_data() const = 0;

    inline void enqueue(const any_tuple& msg) {
        enqueue({invalid_actor_addr, nullptr}, msg);
    }

 protected:

    peer(native_socket_type read_fd,
         native_socket_type write_fd = invalid_socket,
         node_id_ptr peer_ptr = nullptr);

    inline default_message_queue& queue() {
        CPPA_REQUIRE(m_queue != nullptr);
        return *m_queue;
    }

    inline void stop_on_last_proxy_exited(bool value) {
        m_stop_on_last_proxy_exited = value;
    }

 private:

    // called by middleman_impl
    inline void set_queue(const default_message_queue_ptr& queue) {
        m_queue = queue;
    }

    // if this peer was created using remote_actor(), then m_doorman will
    // point to the published actor of the remote node
    bool m_stop_on_last_proxy_exited;

    node_id_ptr m_node;

    default_message_queue_ptr m_queue;

};

} } // namespace cppa::network

#endif // CPPA_peer_IMPL_HPP