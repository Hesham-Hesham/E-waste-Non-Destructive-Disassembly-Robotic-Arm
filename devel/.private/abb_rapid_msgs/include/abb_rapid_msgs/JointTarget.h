// Generated by gencpp from file abb_rapid_msgs/JointTarget.msg
// DO NOT EDIT!


#ifndef ABB_RAPID_MSGS_MESSAGE_JOINTTARGET_H
#define ABB_RAPID_MSGS_MESSAGE_JOINTTARGET_H


#include <string>
#include <vector>
#include <memory>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>

#include <abb_rapid_msgs/RobJoint.h>
#include <abb_rapid_msgs/ExtJoint.h>

namespace abb_rapid_msgs
{
template <class ContainerAllocator>
struct JointTarget_
{
  typedef JointTarget_<ContainerAllocator> Type;

  JointTarget_()
    : robax()
    , extax()  {
    }
  JointTarget_(const ContainerAllocator& _alloc)
    : robax(_alloc)
    , extax(_alloc)  {
  (void)_alloc;
    }



   typedef  ::abb_rapid_msgs::RobJoint_<ContainerAllocator>  _robax_type;
  _robax_type robax;

   typedef  ::abb_rapid_msgs::ExtJoint_<ContainerAllocator>  _extax_type;
  _extax_type extax;





  typedef boost::shared_ptr< ::abb_rapid_msgs::JointTarget_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::abb_rapid_msgs::JointTarget_<ContainerAllocator> const> ConstPtr;

}; // struct JointTarget_

typedef ::abb_rapid_msgs::JointTarget_<std::allocator<void> > JointTarget;

typedef boost::shared_ptr< ::abb_rapid_msgs::JointTarget > JointTargetPtr;
typedef boost::shared_ptr< ::abb_rapid_msgs::JointTarget const> JointTargetConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::abb_rapid_msgs::JointTarget_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::abb_rapid_msgs::JointTarget_<ContainerAllocator> >::stream(s, "", v);
return s;
}


template<typename ContainerAllocator1, typename ContainerAllocator2>
bool operator==(const ::abb_rapid_msgs::JointTarget_<ContainerAllocator1> & lhs, const ::abb_rapid_msgs::JointTarget_<ContainerAllocator2> & rhs)
{
  return lhs.robax == rhs.robax &&
    lhs.extax == rhs.extax;
}

template<typename ContainerAllocator1, typename ContainerAllocator2>
bool operator!=(const ::abb_rapid_msgs::JointTarget_<ContainerAllocator1> & lhs, const ::abb_rapid_msgs::JointTarget_<ContainerAllocator2> & rhs)
{
  return !(lhs == rhs);
}


} // namespace abb_rapid_msgs

namespace ros
{
namespace message_traits
{





template <class ContainerAllocator>
struct IsMessage< ::abb_rapid_msgs::JointTarget_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::abb_rapid_msgs::JointTarget_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::abb_rapid_msgs::JointTarget_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::abb_rapid_msgs::JointTarget_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::abb_rapid_msgs::JointTarget_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::abb_rapid_msgs::JointTarget_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::abb_rapid_msgs::JointTarget_<ContainerAllocator> >
{
  static const char* value()
  {
    return "a80f8974c2ebed9e2ef462fcc2a28a38";
  }

  static const char* value(const ::abb_rapid_msgs::JointTarget_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0xa80f8974c2ebed9eULL;
  static const uint64_t static_value2 = 0x2ef462fcc2a28a38ULL;
};

template<class ContainerAllocator>
struct DataType< ::abb_rapid_msgs::JointTarget_<ContainerAllocator> >
{
  static const char* value()
  {
    return "abb_rapid_msgs/JointTarget";
  }

  static const char* value(const ::abb_rapid_msgs::JointTarget_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::abb_rapid_msgs::JointTarget_<ContainerAllocator> >
{
  static const char* value()
  {
    return "#-------------------------------------------------------------------------------\n"
"# Description:\n"
"#   The purpose of this message definition, is to represent the\n"
"#   RAPID data type \"jointtarget\" (joint position data).\n"
"#\n"
"# Note:\n"
"#   Please see the \"Technical reference manual - RAPID Instructions,\n"
"#   Functions and Data types\" for more details.\n"
"#-------------------------------------------------------------------------------\n"
"\n"
"#-------------------------------------------------------------------------------\n"
"# Message fields\n"
"#-------------------------------------------------------------------------------\n"
"# Axis positions of the robot axes in degrees.\n"
"RobJoint robax\n"
"\n"
"# The position of the external axes.\n"
"ExtJoint extax\n"
"\n"
"================================================================================\n"
"MSG: abb_rapid_msgs/RobJoint\n"
"#-------------------------------------------------------------------------------\n"
"# Description:\n"
"#   The purpose of this message definition, is to represent the\n"
"#   RAPID data type \"robjoint\" (joint position of robot axes).\n"
"#\n"
"# Note:\n"
"#   Please see the \"Technical reference manual - RAPID Instructions,\n"
"#   Functions and Data types\" for more details.\n"
"#-------------------------------------------------------------------------------\n"
"\n"
"#-------------------------------------------------------------------------------\n"
"# Message fields\n"
"#-------------------------------------------------------------------------------\n"
"# The position of robot axis 1-6 in degrees from the calibration position.\n"
"float32 rax_1\n"
"float32 rax_2\n"
"float32 rax_3\n"
"float32 rax_4\n"
"float32 rax_5\n"
"float32 rax_6\n"
"\n"
"================================================================================\n"
"MSG: abb_rapid_msgs/ExtJoint\n"
"#-------------------------------------------------------------------------------\n"
"# Description:\n"
"#   The purpose of this message definition, is to represent the\n"
"#   RAPID data type \"extjoint\" (position of external joints).\n"
"#\n"
"# Note:\n"
"#   Please see the \"Technical reference manual - RAPID Instructions,\n"
"#   Functions and Data types\" for more details.\n"
"#-------------------------------------------------------------------------------\n"
"\n"
"#-------------------------------------------------------------------------------\n"
"# Message fields\n"
"#-------------------------------------------------------------------------------\n"
"# The position of the external logical axis “a”-\"f\",\n"
"# expressed in degrees or mm (depending on the type of axis).\n"
"float32 eax_a\n"
"float32 eax_b\n"
"float32 eax_c\n"
"float32 eax_d\n"
"float32 eax_e\n"
"float32 eax_f\n"
;
  }

  static const char* value(const ::abb_rapid_msgs::JointTarget_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::abb_rapid_msgs::JointTarget_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.robax);
      stream.next(m.extax);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER
  }; // struct JointTarget_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::abb_rapid_msgs::JointTarget_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::abb_rapid_msgs::JointTarget_<ContainerAllocator>& v)
  {
    s << indent << "robax: ";
    s << std::endl;
    Printer< ::abb_rapid_msgs::RobJoint_<ContainerAllocator> >::stream(s, indent + "  ", v.robax);
    s << indent << "extax: ";
    s << std::endl;
    Printer< ::abb_rapid_msgs::ExtJoint_<ContainerAllocator> >::stream(s, indent + "  ", v.extax);
  }
};

} // namespace message_operations
} // namespace ros

#endif // ABB_RAPID_MSGS_MESSAGE_JOINTTARGET_H