#include "catch.hpp"

#include "node.h"
#include "pipe.h"
#include "settings.h"

// Check pipe class
TEST_CASE("Pipe is checked", "[Pipe]") {
  // Tolerance
  const double tolerance = 1.e-12;

  // Node indices
  const unsigned nodeid1 = 100;
  const unsigned nodeid2 = 101;

  // Nodal coordinates
  const Eigen::Vector3d coords1(10.0, 20.0, 30.0);
  const Eigen::Vector3d coords2(11.1, 21.1, 31.1);

  // Creat an array of two node pointers with previous defined index and
  // coordinates of the nodes
  std::array<std::shared_ptr<pipenetwork::Node>, 2> nodes;
  nodes.at(0) = std::make_shared<pipenetwork::Node>(nodeid1, coords1);
  nodes.at(1) = std::make_shared<pipenetwork::Node>(nodeid2, coords2);

  // Pipe index
  const unsigned pipeid = 200;

  // Creat a pipe based on previous created node pointers
  auto pipe = std::make_unique<pipenetwork::Pipe>(pipeid, nodes);

  // Pipe length
  // To calculate length between two points in 3d space
  // length = sqrt(pow(dx,2)+pow(dy,2)+pow(dz,2))
  const double length = sqrt(3 * pow(1.1, 2));

  // Check pipe id
  REQUIRE(pipe->id() == pipeid);

  // Check pipe length
  REQUIRE(pipe->length() == Approx(length).epsilon(tolerance));

  // Check pipe broken status and initialized status
  REQUIRE(pipe->isbroken() == false);

  // Check radius, discharge, max flow velocity and Darcy friction factor of the
  // pipe
  SECTION(
      "Check radius, discharge, max flow velocity and Darcy friction factor of "
      "the pipe") {

    // Radius of the pipe in m
    const double radius = 10.0;

    // Maximum allowable flow velocity of the pipe in m/min
    const double max_velocity = 100.0;

    // Maximum allowable discharge of the pipe in m3/min
    // Calculated by max_discharge=M_PI*pow(radius,2)*max_velocity
    const double max_discharge = M_PI * 1.e4;

    // Water head at two nodes in m
    const double head1 = 110.0;
    const double head2 = 100.0;

    // Dimensionless Darcy friction factor
    const double darcy_friction = 0.1;

    // Calculated discharge in pipe in m3/min using Darcy-Weisbach head loss
    // equation
    const double discharge =
        sqrt(10 * pow(M_PI, 2) * 9.81 * pow(2 * 10, 5) / (8 * 0.1));

    // Assign defined variables to nodes and pipe
    nodes.at(0)->head(head1);
    nodes.at(1)->head(head2);
    pipe->radius(radius);
    pipe->max_velocity(max_velocity);
    pipe->darcy_friction(darcy_friction);

    // Check radius and max flow velocity of the pipe
    REQUIRE(pipe->max_discharge() == Approx(max_discharge).epsilon(tolerance));
    // Check discharge and Darcy friction factor of the pipe
    REQUIRE(pipe->discharge() == Approx(discharge).epsilon(tolerance));
  }

  // Check return pointer to const Node
  SECTION("Check return pointer to const Node") {

    // Check return ids of the nodes
    REQUIRE(pipe->nodes().at(0)->id() == 100);
    REQUIRE(pipe->nodes().at(1)->id() == 101);
    // Check return head assignment status
    REQUIRE(pipe->nodes().at(0)->ishead() == false);
    REQUIRE(pipe->nodes().at(1)->ishead() == false);
  }
}
