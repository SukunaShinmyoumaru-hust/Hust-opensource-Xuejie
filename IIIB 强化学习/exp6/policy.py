import numpy as np

class SoftmaxPolicy:
    def __init__(self, size=5, feature_dim=3, lr=0.001, seed=42):
        self.size = size
        self.feature_dim = feature_dim
        self.lr = lr
        self.rng = np.random.default_rng(seed)
        # Initialize theta: shape (num_actions, feature_dim)
        # Normal distribution with mean 0, std 0.1
        self.theta = self.rng.normal(0, 0.1, size=(4, feature_dim))

    def get_features(self, s):
        """
        Extract features for state s.
        s: state index (0 to 24)
        Returns: phi(s) = [1, x, y]^T where x = row + 1, y = col + 1
        """
        row, col = divmod(s, self.size)
        x = row + 1.0
        y = col + 1.0
        return np.array([1.0, x, y])

    def get_preferences(self, s):
        """
        Compute action preferences h(s, a, theta) = phi(s)^T * theta_a
        Returns: array of shape (4,)
        """
        phi = self.get_features(s)
        # self.theta is (4, 3), phi is (3,) -> dot product is (4,)
        return np.dot(self.theta, phi)

    def get_probs(self, s):
        """
        Compute action probabilities using Softmax.
        Returns: array of shape (4,)
        """
        h = self.get_preferences(s)
        h_stable = h - np.max(h)  # For numerical stability
        exp_h = np.exp(h_stable)
        probs = exp_h / np.sum(exp_h)
        return probs

    def choose_action(self, s):
        """
        Choose an action according to the policy probabilities.
        """
        probs = self.get_probs(s)
        return self.rng.choice(4, p=probs)

    def update_step(self, s, a, score):
        """
        Perform a single gradient ascent step for a state, action, and score value.
        """
        phi = self.get_features(s)
        probs = self.get_probs(s)
        # Update each parameter vector theta_a'
        for a_prime in range(4):
            indicator = 1.0 if a_prime == a else 0.0
            grad = phi * (indicator - probs[a_prime])
            self.theta[a_prime] += self.lr * score * grad

    def update_trajectory(self, trajectory, gamma=0.9):
        """
        Update policy parameters from one complete episode using REINFORCE.
        trajectory: list of tuples (s_t, a_t, r_{t+1})
        """
        T = len(trajectory)
        # Compute returns q_t for each step t
        returns = np.zeros(T)
        g = 0.0
        for t in reversed(range(T)):
            r = trajectory[t][2]
            g = r + gamma * g
            returns[t] = g

        # Update parameters
        for t in range(T):
            s_t, a_t, _ = trajectory[t]
            score = (gamma ** t) * returns[t]
            self.update_step(s_t, a_t, score)
