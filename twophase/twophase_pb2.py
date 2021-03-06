# -*- coding: utf-8 -*-
# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: twophase.proto

import sys
_b=sys.version_info[0]<3 and (lambda x:x) or (lambda x:x.encode('latin1'))
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from google.protobuf import reflection as _reflection
from google.protobuf import symbol_database as _symbol_database
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()




DESCRIPTOR = _descriptor.FileDescriptor(
  name='twophase.proto',
  package='',
  syntax='proto3',
  serialized_options=None,
  serialized_pb=_b('\n\x0etwophase.proto\"\x12\n\x10GetCommittedArgs\"\x07\n\x05\x45mpty\"0\n\nMaybeValue\x12\x11\n\tavailable\x18\x01 \x01(\x08\x12\x0f\n\x07\x63ontent\x18\x02 \x01(\t\"7\n\x05MyMsg\x12\x11\n\tavailable\x18\x01 \x01(\x08\x12\x0f\n\x07\x63ontent\x18\x02 \x01(\t\x12\n\n\x02id\x18\x03 \x01(\x05\x32r\n\x06Worker\x12\x30\n\x0cGetCommitted\x12\x11.GetCommittedArgs\x1a\x0b.MaybeValue\"\x00\x12\x1c\n\x08SetValue\x12\x06.MyMsg\x1a\x06.Empty\"\x00\x12\x18\n\x04Vote\x12\x06.MyMsg\x1a\x06.Empty\"\x00\x32\x30\n\x0b\x43oordinator\x12!\n\x08SetValue\x12\x0b.MaybeValue\x1a\x06.Empty\"\x00\x62\x06proto3')
)




_GETCOMMITTEDARGS = _descriptor.Descriptor(
  name='GetCommittedArgs',
  full_name='GetCommittedArgs',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  serialized_options=None,
  is_extendable=False,
  syntax='proto3',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=18,
  serialized_end=36,
)


_EMPTY = _descriptor.Descriptor(
  name='Empty',
  full_name='Empty',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  serialized_options=None,
  is_extendable=False,
  syntax='proto3',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=38,
  serialized_end=45,
)


_MAYBEVALUE = _descriptor.Descriptor(
  name='MaybeValue',
  full_name='MaybeValue',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='available', full_name='MaybeValue.available', index=0,
      number=1, type=8, cpp_type=7, label=1,
      has_default_value=False, default_value=False,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='content', full_name='MaybeValue.content', index=1,
      number=2, type=9, cpp_type=9, label=1,
      has_default_value=False, default_value=_b("").decode('utf-8'),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  serialized_options=None,
  is_extendable=False,
  syntax='proto3',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=47,
  serialized_end=95,
)


_MYMSG = _descriptor.Descriptor(
  name='MyMsg',
  full_name='MyMsg',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='available', full_name='MyMsg.available', index=0,
      number=1, type=8, cpp_type=7, label=1,
      has_default_value=False, default_value=False,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='content', full_name='MyMsg.content', index=1,
      number=2, type=9, cpp_type=9, label=1,
      has_default_value=False, default_value=_b("").decode('utf-8'),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='id', full_name='MyMsg.id', index=2,
      number=3, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  serialized_options=None,
  is_extendable=False,
  syntax='proto3',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=97,
  serialized_end=152,
)

DESCRIPTOR.message_types_by_name['GetCommittedArgs'] = _GETCOMMITTEDARGS
DESCRIPTOR.message_types_by_name['Empty'] = _EMPTY
DESCRIPTOR.message_types_by_name['MaybeValue'] = _MAYBEVALUE
DESCRIPTOR.message_types_by_name['MyMsg'] = _MYMSG
_sym_db.RegisterFileDescriptor(DESCRIPTOR)

GetCommittedArgs = _reflection.GeneratedProtocolMessageType('GetCommittedArgs', (_message.Message,), {
  'DESCRIPTOR' : _GETCOMMITTEDARGS,
  '__module__' : 'twophase_pb2'
  # @@protoc_insertion_point(class_scope:GetCommittedArgs)
  })
_sym_db.RegisterMessage(GetCommittedArgs)

Empty = _reflection.GeneratedProtocolMessageType('Empty', (_message.Message,), {
  'DESCRIPTOR' : _EMPTY,
  '__module__' : 'twophase_pb2'
  # @@protoc_insertion_point(class_scope:Empty)
  })
_sym_db.RegisterMessage(Empty)

MaybeValue = _reflection.GeneratedProtocolMessageType('MaybeValue', (_message.Message,), {
  'DESCRIPTOR' : _MAYBEVALUE,
  '__module__' : 'twophase_pb2'
  # @@protoc_insertion_point(class_scope:MaybeValue)
  })
_sym_db.RegisterMessage(MaybeValue)

MyMsg = _reflection.GeneratedProtocolMessageType('MyMsg', (_message.Message,), {
  'DESCRIPTOR' : _MYMSG,
  '__module__' : 'twophase_pb2'
  # @@protoc_insertion_point(class_scope:MyMsg)
  })
_sym_db.RegisterMessage(MyMsg)



_WORKER = _descriptor.ServiceDescriptor(
  name='Worker',
  full_name='Worker',
  file=DESCRIPTOR,
  index=0,
  serialized_options=None,
  serialized_start=154,
  serialized_end=268,
  methods=[
  _descriptor.MethodDescriptor(
    name='GetCommitted',
    full_name='Worker.GetCommitted',
    index=0,
    containing_service=None,
    input_type=_GETCOMMITTEDARGS,
    output_type=_MAYBEVALUE,
    serialized_options=None,
  ),
  _descriptor.MethodDescriptor(
    name='SetValue',
    full_name='Worker.SetValue',
    index=1,
    containing_service=None,
    input_type=_MYMSG,
    output_type=_EMPTY,
    serialized_options=None,
  ),
  _descriptor.MethodDescriptor(
    name='Vote',
    full_name='Worker.Vote',
    index=2,
    containing_service=None,
    input_type=_MYMSG,
    output_type=_EMPTY,
    serialized_options=None,
  ),
])
_sym_db.RegisterServiceDescriptor(_WORKER)

DESCRIPTOR.services_by_name['Worker'] = _WORKER


_COORDINATOR = _descriptor.ServiceDescriptor(
  name='Coordinator',
  full_name='Coordinator',
  file=DESCRIPTOR,
  index=1,
  serialized_options=None,
  serialized_start=270,
  serialized_end=318,
  methods=[
  _descriptor.MethodDescriptor(
    name='SetValue',
    full_name='Coordinator.SetValue',
    index=0,
    containing_service=None,
    input_type=_MAYBEVALUE,
    output_type=_EMPTY,
    serialized_options=None,
  ),
])
_sym_db.RegisterServiceDescriptor(_COORDINATOR)

DESCRIPTOR.services_by_name['Coordinator'] = _COORDINATOR

# @@protoc_insertion_point(module_scope)
