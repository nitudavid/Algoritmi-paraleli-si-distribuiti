package shortestPathsFloyd_Warshall;

import java.util.concurrent.BrokenBarrierException;

public class MyThread implements Runnable {
    int id, start, end, N, P;
    int[][] graph;

    public MyThread(int id, int N, int P, int[][] graph) {
        this.id = id;
        this.N = N;
        this.P = P;
        this.graph = graph;

        start = id * N / P;
        end = Integer.min((id + 1) * N / P, N);
    }

    @Override
    public void run() {
        for (int k = 0; k < N; k++) {
            for (int i = start; i < end; i++) {
                for (int j = 0; j < N; j++) {
                    graph[i][j] = Math.min(graph[i][k] + graph[k][j], graph[i][j]);
                }
            }

            try {
                Main.barrier.await();
            } catch (InterruptedException | BrokenBarrierException e) {
                e.printStackTrace();
            }
        }
    }
}
