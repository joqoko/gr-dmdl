
/*
 * This file was automatically generated using swig_doc.py.
 *
 * Any changes to it will be lost next time it is regenerated.
 */




%feature("docstring") gr::dmdl::backoff "<+description of block+>

Constructor Specific Documentation:

Return a shared_ptr to a new instance of dmdl::backoff.

To avoid accidental use of raw pointers, dmdl::backoff's constructor is in a private implementation class. dmdl::backoff::make is the public interface for creating new instances.

Args:
    develop_mode : 
    block_id : 
    backoff_type : 
    backoff_time_unit_ms : 
    min_backoff_ms : 
    max_backoff_ms : 
    apply_cs : 
    cs_threshold : 
    system_time_granularity_us : 
    virtual_cs : 
    min_window_size : "

%feature("docstring") gr::dmdl::backoff::make "<+description of block+>

Constructor Specific Documentation:

Return a shared_ptr to a new instance of dmdl::backoff.

To avoid accidental use of raw pointers, dmdl::backoff's constructor is in a private implementation class. dmdl::backoff::make is the public interface for creating new instances.

Args:
    develop_mode : 
    block_id : 
    backoff_type : 
    backoff_time_unit_ms : 
    min_backoff_ms : 
    max_backoff_ms : 
    apply_cs : 
    cs_threshold : 
    system_time_granularity_us : 
    virtual_cs : 
    min_window_size : "

%feature("docstring") gr::dmdl::frame_analysis "<+description of block+>

Constructor Specific Documentation:

Return a shared_ptr to a new instance of dmdl::frame_analysis.

To avoid accidental use of raw pointers, dmdl::frame_analysis's constructor is in a private implementation class. dmdl::frame_analysis::make is the public interface for creating new instances.

Args:
    develop_mode : 
    block_id : 
    len_frame_type : 
    len_frame_index : 
    len_destination_address : 
    len_source_address : 
    len_num_transmission : 
    len_reserved_field_I : 
    len_reserved_field_II : 
    len_payload_length : 
    apply_self_address_check : 
    my_address : "

%feature("docstring") gr::dmdl::frame_analysis::make "<+description of block+>

Constructor Specific Documentation:

Return a shared_ptr to a new instance of dmdl::frame_analysis.

To avoid accidental use of raw pointers, dmdl::frame_analysis's constructor is in a private implementation class. dmdl::frame_analysis::make is the public interface for creating new instances.

Args:
    develop_mode : 
    block_id : 
    len_frame_type : 
    len_frame_index : 
    len_destination_address : 
    len_source_address : 
    len_num_transmission : 
    len_reserved_field_I : 
    len_reserved_field_II : 
    len_payload_length : 
    apply_self_address_check : 
    my_address : "

%feature("docstring") gr::dmdl::framing "<+description of block+>

Constructor Specific Documentation:

Return a shared_ptr to a new instance of dmdl::framing.

To avoid accidental use of raw pointers, dmdl::framing's constructor is in a private implementation class. dmdl::framing::make is the public interface for creating new instances.

Args:
    develop_mode : 
    block_id : 
    frame_type : 
    len_frame_type : 
    frame_index : 
    len_frame_index : 
    destination_address : 
    len_destination_address : 
    source_address : 
    len_source_address : 
    reserved_field_I : 
    len_reserved_field_I : 
    reserved_field_II : 
    len_reserved_field_II : 
    len_payload_length : 
    increase_index : 
    len_num_transmission : 
    reserved_field_ampdu : 
    node_list : 
    slot_list_ms : 
    len_slot_time_beacon : 
    default_payload : 
    default_payload_length : 
    internal_index : "

%feature("docstring") gr::dmdl::framing::make "<+description of block+>

Constructor Specific Documentation:

Return a shared_ptr to a new instance of dmdl::framing.

To avoid accidental use of raw pointers, dmdl::framing's constructor is in a private implementation class. dmdl::framing::make is the public interface for creating new instances.

Args:
    develop_mode : 
    block_id : 
    frame_type : 
    len_frame_type : 
    frame_index : 
    len_frame_index : 
    destination_address : 
    len_destination_address : 
    source_address : 
    len_source_address : 
    reserved_field_I : 
    len_reserved_field_I : 
    reserved_field_II : 
    len_reserved_field_II : 
    len_payload_length : 
    increase_index : 
    len_num_transmission : 
    reserved_field_ampdu : 
    node_list : 
    slot_list_ms : 
    len_slot_time_beacon : 
    default_payload : 
    default_payload_length : 
    internal_index : "

%feature("docstring") gr::dmdl::timeout "<+description of block+>

Constructor Specific Documentation:

Return a shared_ptr to a new instance of dmdl::timeout.

To avoid accidental use of raw pointers, dmdl::timeout's constructor is in a private implementation class. dmdl::timeout::make is the public interface for creating new instances.

Args:
    develop_mode : 
    block_id : 
    timeout_duration_ms : 
    system_time_granularity_us : 
    llc_protocol : "

%feature("docstring") gr::dmdl::timeout::make "<+description of block+>

Constructor Specific Documentation:

Return a shared_ptr to a new instance of dmdl::timeout.

To avoid accidental use of raw pointers, dmdl::timeout's constructor is in a private implementation class. dmdl::timeout::make is the public interface for creating new instances.

Args:
    develop_mode : 
    block_id : 
    timeout_duration_ms : 
    system_time_granularity_us : 
    llc_protocol : "

%feature("docstring") gr::dmdl::timer "<+description of block+>

Constructor Specific Documentation:

Return a shared_ptr to a new instance of dmdl::timer.

To avoid accidental use of raw pointers, dmdl::timer's constructor is in a private implementation class. dmdl::timer::make is the public interface for creating new instances.

Args:
    develop_mode : 
    block_id : 
    timer_type : 
    duration_ms : 
    system_time_granularity_us : 
    reserved_time_ms : "

%feature("docstring") gr::dmdl::timer::make "<+description of block+>

Constructor Specific Documentation:

Return a shared_ptr to a new instance of dmdl::timer.

To avoid accidental use of raw pointers, dmdl::timer's constructor is in a private implementation class. dmdl::timer::make is the public interface for creating new instances.

Args:
    develop_mode : 
    block_id : 
    timer_type : 
    duration_ms : 
    system_time_granularity_us : 
    reserved_time_ms : "